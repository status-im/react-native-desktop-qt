
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
#include <QString>
#include <QVariant>

#include <QDebug>

#include "layout/flexbox.h"
#include "reactattachedproperties.h"
#include "reactbridge.h"
#include "reactitem.h"
#include "reactpropertyhandler.h"
#include "reactscrollviewmanager.h"
#include "reactuimanager.h"
#include "utilities.h"
#include "utilities.h"

using namespace utilities;

void ReactScrollViewManager::scrollTo(int reactTag, double offsetX, double offsetY, bool animated) {
    QQuickItem* item = bridge()->uiManager()->viewForTag(reactTag);
    Q_ASSERT(item != nullptr);

    QQmlProperty(item, "contentX").write(offsetX);
    QQmlProperty(item, "contentY").write(offsetY);
}

ReactScrollViewManager::ReactScrollViewManager(QObject* parent) : ReactViewManager(parent) {}

ReactScrollViewManager::~ReactScrollViewManager() {}

ReactViewManager* ReactScrollViewManager::viewManager() {
    return this;
}

QString ReactScrollViewManager::moduleName() {
    return "RCTScrollViewManager";
}

QStringList ReactScrollViewManager::customDirectEventTypes() {
    return QStringList{"scrollBeginDrag",
                       normalizeInputEventName("onScroll"),
                       "scrollEndDrag",
                       "scrollAnimationEnd",
                       "momentumScrollBegin",
                       "momentumScrollEnd"};
}

void ReactScrollViewManager::addChildItem(QQuickItem* scrollView, QQuickItem* child, int position) const {
    // add to parents content item
    QQuickItem* contentItem = QQmlProperty(scrollView, "contentItem").read().value<QQuickItem*>();
    Q_ASSERT(contentItem != nullptr);
    child->setParentItem(contentItem);
}

void ReactScrollViewManager::scrollBeginDrag() {
    // qDebug() << __PRETTY_FUNCTION__;
    QQuickItem* item = qobject_cast<QQuickItem*>(sender());
    Q_ASSERT(item != nullptr);
    notifyJsAboutEvent(tag(item), "scrollBeginDrag", {});
}

void ReactScrollViewManager::scrollEndDrag() {
    // qDebug() << __PRETTY_FUNCTION__;
    QQuickItem* item = qobject_cast<QQuickItem*>(sender());
    Q_ASSERT(item != nullptr);
    notifyJsAboutEvent(tag(item), "scrollEndDrag", {});
}

void ReactScrollViewManager::scroll() {
    QQuickItem* item = qobject_cast<QQuickItem*>(sender());
    Q_ASSERT(item != nullptr);

    bool scrollFlagSet = item->property("p_onScroll").toBool();

    if (scrollFlagSet) {
        notifyJsAboutEvent(tag(item), "onScroll", buildEventData(item));
    }
}

void ReactScrollViewManager::momentumScrollBegin() {
    // qDebug() << __PRETTY_FUNCTION__;
    QQuickItem* item = qobject_cast<QQuickItem*>(sender());
    Q_ASSERT(item != nullptr);
    notifyJsAboutEvent(tag(item), "momentumScrollBegin", buildEventData(item));
}

void ReactScrollViewManager::momentumScrollEnd() {
    // qDebug() << __PRETTY_FUNCTION__;
    QQuickItem* item = qobject_cast<QQuickItem*>(sender());
    Q_ASSERT(item != nullptr);
    notifyJsAboutEvent(tag(item), "momentumScrollEnd", buildEventData(item));
}

namespace {
template <typename TP> TP propertyValue(QQuickItem* item, const QString& property) {
    return QQmlProperty(item, property).read().value<TP>();
}
}

QVariantMap ReactScrollViewManager::buildEventData(QQuickItem* item) const {
    QVariantMap ed;
    ed.insert("contentOffset",
              QVariantMap{
                  {"x", propertyValue<double>(item, "contentX")}, {"y", propertyValue<double>(item, "contentY")},
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

void ReactScrollViewManager::configureView(QQuickItem* view) const {
    ReactViewManager::configureView(view);
    // This would be prettier with a Functor version, but connect doesnt support it
    connect(view, SIGNAL(movementStarted()), SLOT(scrollBeginDrag()));
    connect(view, SIGNAL(movementEnded()), SLOT(scrollEndDrag()));
    connect(view, SIGNAL(movingChanged()), SLOT(scroll()));

    connect(view, SIGNAL(flickStarted()), SLOT(momentumScrollBegin()));
    connect(view, SIGNAL(flickEnded()), SLOT(momentumScrollEnd()));
}

QString ReactScrollViewManager::qmlComponentFile() const {
    return ":/qml/ReactScrollView.qml";
}

#include "reactscrollviewmanager.moc"
