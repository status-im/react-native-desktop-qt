
/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * Author: Justin McPherson <justin.mcpherson@canonical.com>
 *
 */

#include <QQmlComponent>
#include <QQuickItem>
#include <QSet>
#include <QString>
#include <QVariant>
#include <QVariantList>
#include <QWheelEvent>

#include <QDebug>

#include "attachedproperties.h"
#include "bridge.h"
#include "layout/flexbox.h"
#include "propertyhandler.h"
#include "reactitem.h"
#include "scrollviewmanager.h"
#include "uimanager.h"
#include "utilities.h"

using namespace utilities;

QMap<QQuickItem*, QQuickItem*> ScrollViewManager::m_scrollViewByListViewItem;
QMap<QQuickItem*, ScrollViewModelPtr> ScrollViewManager::m_modelByScrollView;
QMap<QQuickItem*, QTimer*> ScrollViewManager::m_scrollTimers;

void ScrollViewManager::scrollTo(int reactTag, double offsetX, double offsetY, bool animated) {
    QQuickItem* item = bridge()->uiManager()->viewForTag(reactTag);
    Q_ASSERT(item != nullptr);

    QQmlProperty(item, "contentX").write(offsetX);
    QQmlProperty(item, "contentY").write(offsetY);
}

void ScrollViewManager::scrollToEnd(int reactTag, bool animated) {
    QQuickItem* item = bridge()->uiManager()->viewForTag(reactTag);
    Q_ASSERT(item != nullptr);

    if (arrayScrollingOptimizationEnabled(item)) {
        int scrollViewModelItemsCount = item->property("count").toInt();
        QMetaObject::invokeMethod(item, "positionViewAtEnd");
        QQmlProperty(item, "currentIndex").write(scrollViewModelItemsCount - 1);
        QMetaObject::invokeMethod(item, "positionViewAtEnd");
    } else {
        qreal contentHeight = item->property("contentHeight").toReal();
        qreal height = item->property("height").toReal();
        qreal newContentY = (contentHeight > height) ? contentHeight - height : 0;
        QQmlProperty(item, "contentY").write(newContentY);
    }
}

ScrollViewManager::ScrollViewManager(QObject* parent) : ViewManager(parent) {}

ScrollViewManager::~ScrollViewManager() {}

ViewManager* ScrollViewManager::viewManager() {
    return this;
}

QString ScrollViewManager::moduleName() {
    return "RCTScrollViewManager";
}

QStringList ScrollViewManager::customDirectEventTypes() {
    return QStringList{normalizeInputEventName("scrollBeginDrag"),
                       normalizeInputEventName("onScroll"),
                       normalizeInputEventName("scrollEndDrag"),
                       normalizeInputEventName("scrollAnimationEnd"),
                       normalizeInputEventName("momentumScrollBegin"),
                       normalizeInputEventName("momentumScrollEnd")};
}

bool ScrollViewManager::isArrayScrollingOptimizationEnabled(QQuickItem* item) {
    return m_scrollViewByListViewItem.contains(item);
}

void ScrollViewManager::updateListViewItem(QQuickItem* item, QQuickItem* child, int position) {
    QQuickItem* scrollView = m_scrollViewByListViewItem[item];
    ScrollViewModelPtr model = m_modelByScrollView[scrollView];
    model->insert(child, position);
    QQmlProperty::write(scrollView, "model", QVariant::fromValue(model.data()));
}

QList<QQuickItem*> ScrollViewManager::removeListViewItems(QQuickItem* item, const QList<int>& removeAtIndices) {

    QList<QQuickItem*> removedChildren;

    if (removeAtIndices.isEmpty())
        return removedChildren;

    QQuickItem* scrollView = m_scrollViewByListViewItem[item];
    ScrollViewModelPtr model = m_modelByScrollView[scrollView];

    foreach (int idxToRemote, removeAtIndices) {
        QQuickItem* itemToRemove = model->takeAt(idxToRemote).value<QQuickItem*>();
        itemToRemove->setParentItem(nullptr);
        removedChildren.push_back(itemToRemove);
    }

    utilities::removeFlexboxChilds(item, removeAtIndices);

    return removedChildren;
}

QQuickItem* ScrollViewManager::scrollViewContentItem(QQuickItem* item, int position) {
    QQuickItem* scrollView = m_scrollViewByListViewItem[item];
    ScrollViewModelPtr model = m_modelByScrollView[scrollView];

    Q_ASSERT(position < model->count());
    return model->data(model->index(position)).value<QQuickItem*>();
}

void ScrollViewManager::addChildItem(QQuickItem* scrollView, QQuickItem* child, int position) const {
    if (arrayScrollingOptimizationEnabled(scrollView)) {
        if (!m_modelByScrollView.contains(scrollView)) {
            m_modelByScrollView[scrollView] = ScrollViewModelPtr(new ScrollViewModel(bridge()->qmlEngine()));
        }

        ScrollViewModelPtr model = m_modelByScrollView[scrollView];
        foreach (QQuickItem* item, child->childItems()) { model->insert(item, model->count()); }
        QQmlProperty::write(scrollView, "model", QVariant::fromValue(model.data()));
        m_scrollViewByListViewItem.insert(child, scrollView);
    } else {
        // Flickable items should be children of contentItem
        QQuickItem* contentItem = QQmlProperty(scrollView, "contentItem").read().value<QQuickItem*>();
        Q_ASSERT(contentItem != nullptr);
        utilities::insertChildItemAt(child, position, contentItem);
    }
}

void ScrollViewManager::scrollBeginDrag() {
    // qDebug() << __PRETTY_FUNCTION__;
    QQuickItem* item = qobject_cast<QQuickItem*>(sender());
    Q_ASSERT(item != nullptr);
    notifyJsAboutEvent(tag(item), "scrollBeginDrag", buildEventData(item));
}

void ScrollViewManager::scrollEndDrag() {
    // qDebug() << __PRETTY_FUNCTION__;
    QQuickItem* item = qobject_cast<QQuickItem*>(sender());
    Q_ASSERT(item != nullptr);
    notifyJsAboutEvent(tag(item), "scrollEndDrag", buildEventData(item));
}

void ScrollViewManager::scroll() {
    QQuickItem* item = qobject_cast<QQuickItem*>(sender());
    Q_ASSERT(item != nullptr);

    QTimer* timer = nullptr;
    if (!m_scrollTimers.contains(item)) {
        timer = new QTimer(this);
        m_scrollTimers.insert(item, timer);
        connect(timer, &QTimer::timeout, this, [=]() {
            bool scrollFlagSet = item->property("p_onScroll").toBool();
            if (scrollFlagSet) {
                notifyJsAboutEvent(tag(item), "onScroll", buildEventData(item));
            }
        });
    } else {
        timer = m_scrollTimers[item];
    }

    bool isMoving = item->property("moving").toBool();
    if (isMoving) {
        timer->start(300);
    } else {
        timer->stop();
    }
}

void ScrollViewManager::onDraggingChanged() {
    QQuickItem* item = qobject_cast<QQuickItem*>(sender());
    Q_ASSERT(item != nullptr);

    bool isDragging = item->property("dragging").toBool();
    if (isDragging) {
        scrollBeginDrag();
    } else {
        scrollEndDrag();
    }
}

void ScrollViewManager::momentumScrollBegin(QQuickItem* item) {
    // qDebug() << __PRETTY_FUNCTION__;
    notifyJsAboutEvent(tag(item), "momentumScrollBegin", buildEventData(item));
}

void ScrollViewManager::momentumScrollEnd(QQuickItem* item) {
    // qDebug() << __PRETTY_FUNCTION__;
    notifyJsAboutEvent(tag(item), "momentumScrollEnd", buildEventData(item));
}

void ScrollViewManager::applyTransformation(QQuickItem* item, QVariantList transform) {

    QVector<float> transformVector;
    foreach (QVariant v, transform) {
        if (v.canConvert(QMetaType::Double)) {
            transformVector << v.value<float>();
        } else {
            return;
        }
    }

    QQmlListReference r(item, "transform");
    if (r.canAppend()) {
        r.clear();
        r.append(new MatrixTransform(transformVector, item));
    }
}

bool ScrollViewManager::eventFilter(QObject* scrollView, QEvent* event) {
#if defined(Q_OS_MACOS)
    static QSet<QWheelEvent*> artificialEvents;

    if (event->type() == QEvent::Wheel) {

        QWheelEvent* e = static_cast<QWheelEvent*>(event);
        bool shouldInvertWheelEvents = scrollView->property("invertedScroll").toBool();

        if (shouldInvertWheelEvents && !artificialEvents.contains(e)) {
            QWheelEvent* modifiedEvent = new QWheelEvent(e->posF(),
                                                         e->globalPosF(),
                                                         -e->pixelDelta(),
                                                         e->angleDelta(),
                                                         e->delta(),
                                                         e->orientation(),
                                                         e->buttons(),
                                                         e->modifiers(),
                                                         e->phase(),
                                                         e->source(),
                                                         true);
            artificialEvents.insert(modifiedEvent);
            QCoreApplication::sendEvent(scrollView, modifiedEvent);
            artificialEvents.remove(modifiedEvent);
            delete modifiedEvent;
            return true;
        }
    }
#endif
    return false;
}

namespace {
template <typename TP> TP propertyValue(QQuickItem* item, const QString& property) {
    return QQmlProperty(item, property).read().value<TP>();
}
} // namespace

QVariantMap ScrollViewManager::buildEventData(QQuickItem* item) const {
    QVariantMap ed;
    ed.insert("contentOffset",
              QVariantMap{
                  {"x", propertyValue<double>(item, "contentX") - propertyValue<double>(item, "originX")},
                  {"y", propertyValue<double>(item, "contentY") - propertyValue<double>(item, "originY")},
              });
    // ed.insert("contentInset", QVariantMap{
    // });
    ed.insert("contentSize",
              QVariantMap{
                  {"width", propertyValue<double>(item, "contentWidth")},
                  {"height", propertyValue<double>(item, "contentHeight")},
              });
    ed.insert("layoutMeasurement",
              QVariantMap{
                  {"width", propertyValue<double>(item, "width")}, {"height", propertyValue<double>(item, "height")},

              });
    ed.insert("zoomScale", 1);
    return ed;
}

void ScrollViewManager::configureView(QQuickItem* view) const {
    ViewManager::configureView(view);
    view->setProperty("scrollViewManager", QVariant::fromValue((QObject*)this));
    // This would be prettier with a Functor version, but connect doesnt support it
    view->installEventFilter((QObject*)this);
    connect(view, SIGNAL(draggingChanged()), SLOT(onDraggingChanged()));
    connect(view, SIGNAL(movingChanged()), SLOT(scroll()));
}

QString ScrollViewManager::qmlComponentFile(const QVariantMap& properties) const {

    return properties.value("enableArrayScrollingOptimization", false).toBool() ? "qrc:/qml/ReactScrollListView.qml"
                                                                                : "qrc:/qml/ReactScrollView.qml";
}

bool ScrollViewManager::arrayScrollingOptimizationEnabled(QQuickItem* item) const {
    return QQmlProperty(item, "p_enableArrayScrollingOptimization").read().toBool();
}

#include "scrollviewmanager.moc"
