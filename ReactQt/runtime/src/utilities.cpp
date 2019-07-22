
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "utilities.h"

#include <QMetaType>
#include <QtQml>

#include "attachedproperties.h"
#include "componentmanagers/imagemanager.h"
#include "componentmanagers/viewmanager.h"
#include "layout/flexbox.h"
#include "mouseeventsinterceptor.h"
#include "reactitem.h"
#include "rootview.h"
#include "utilities.h"

const int MAJOR_VERSION = 0;
const int MINOR_VERSION = 1;

namespace utilities {

MatrixTransform::MatrixTransform(const QVector<float>& transformMatrix, QQuickItem* parent)
    : QQuickTransform(parent), m_item(qobject_cast<QQuickItem*>(parent)) {
    memcpy(m_transformMatrix.data(), transformMatrix.constData(), 16 * sizeof(float));
    m_transformMatrix.optimize();
}
void MatrixTransform::applyTo(QMatrix4x4* matrix) const {
    if (m_transformMatrix.isIdentity())
        return;
    matrix->translate(m_item->width() / 2, m_item->height() / 2);
    *matrix *= m_transformMatrix;
    matrix->translate(-m_item->width() / 2, -m_item->height() / 2);
}

void registerReactTypes() {
    qmlRegisterUncreatableType<AttachedProperties>(
        "React", MAJOR_VERSION, MINOR_VERSION, "React", "React is not meant to be created directly");
    qmlRegisterType<ReactItem>("React", MAJOR_VERSION, MINOR_VERSION, "Item");
    qmlRegisterType<RootView>("React", MAJOR_VERSION, MINOR_VERSION, "RootView");
    qmlRegisterType<MouseEventsInterceptor>("React", MAJOR_VERSION, MINOR_VERSION, "MouseEventsInterceptor");

    qmlRegisterType<Flexbox>("React", MAJOR_VERSION, MINOR_VERSION, "Flexbox");
    qmlRegisterUncreatableType<ImageManager>("React",
                                             MAJOR_VERSION,
                                             MINOR_VERSION,
                                             "ReactImageManager",
                                             "ReactImageManager is not meant to be created directly");
    qmlRegisterUncreatableType<ViewManager>("React",
                                            MAJOR_VERSION,
                                            MINOR_VERSION,
                                            "ReactViewManager",
                                            "ReactViewManager is not meant to be created directly");
}

QString normalizeInputEventName(const QString& eventName) {
    QString tmp = eventName;
    if (eventName.startsWith("top"))
        return tmp;

    if (eventName.startsWith("on")) {
        tmp.replace(0, 2, "top");
    } else {
        tmp[0] = tmp[0].toUpper();
        tmp = "top" + tmp;
    }

    return tmp;
}

QQuickItem* createQMLItemFromSourceFile(QQmlEngine* qmlEngine, const QUrl& fileUrl) {

    QmlComponentPtr component = createComponentFromSourceFile(qmlEngine, fileUrl);
    return createQMLItemFromComponent(component);
}

QmlComponentPtr createComponentFromSourceFile(QQmlEngine* qmlEngine, const QUrl& fileUrl) {
    QmlComponentPtr component = QmlComponentPtr(new QQmlComponent(qmlEngine));
    component->loadUrl(fileUrl);
    if (!component->isReady()) {
        qCritical() << QString("Component for %1 is not loaded").arg(fileUrl.toString());
    }
    return component;
}

QQuickItem* createQMLItemFromComponent(QmlComponentPtr component) {
    QObject* createdObject = component->create();
    if (createdObject == nullptr) {
        qCritical() << QString("Unable to construct item from component %1").arg(component->url().toString());
    }
    return qobject_cast<QQuickItem*>(createdObject);
}

QObject* createQObjectInstance(const QString& typeName) {
    const int connectionType = QMetaType::type((typeName + "*").toLocal8Bit());
    if (connectionType == QMetaType::UnknownType) {
        return nullptr;
    }
    const QMetaObject* mObj = QMetaType::metaObjectForType(connectionType);
    if (mObj == nullptr) {
        return nullptr;
    }
    return mObj->newInstance();
}

void insertChildItemAt(QQuickItem* item, int position, QQuickItem* parent) {
    if (!item || !parent)
        return;

    QList<QQuickItem*> childItems = parent->childItems();
    if (childItems.size() && childItems.size() > position) {
        QQuickItem* nextItem = childItems.at(position);
        item->setParentItem(parent);
        item->setParent(parent);
        item->stackBefore(nextItem);
    } else {
        item->setParentItem(parent);
        item->setParent(parent);
    }
}

void removeFlexboxChilds(QQuickItem* item, const QList<int>& removeAtIndices) {
    auto flexbox = Flexbox::findFlexbox(item);
    if (flexbox) {
        qCDebug(FLEXBOX) << "utilities::removeFlexboxChilds remove children at indexes" << removeAtIndices
                         << " from parent item: " << item << " parent flexbox: " << flexbox;
        flexbox->removeChilds(removeAtIndices);
    }
}

QVariantMap createTouchArgs(int tag, const QPointF& lp, const QPointF& local, const QString& button, ulong timestamp) {
    return QVariantMap{{"target", tag},
                       {"identifier", 1},
                       {"touches", QVariant()},
                       {"changedTouches", QVariant()},
                       {"pageX", lp.x()},
                       {"pageY", lp.y()},
                       {"locationX", local.x()},
                       {"locationY", local.y()},
                       {"button", button},
                       {"timestamp", QVariant::fromValue(timestamp)}};
}

QQuickItem* getChildFromScrollView(QQuickItem* scrollView, const QPointF& scrollViewPos) {
    QQuickItem* itemAt = nullptr;
    QQuickItem* contentItem = scrollView->property("contentItem").value<QQuickItem*>();
    if (!contentItem)
        return nullptr;

    QPointF contentItemPos = scrollView->mapToItem(contentItem, scrollViewPos);
    QMetaObject::invokeMethod(scrollView,
                              "itemAt",
                              Qt::DirectConnection,
                              Q_RETURN_ARG(QQuickItem*, itemAt),
                              Q_ARG(qreal, contentItemPos.x()),
                              Q_ARG(qreal, contentItemPos.y()));

    return itemAt;
}

QQuickItem* getChildFromText(QQuickItem* text, const QPointF& pos) {
    QString linkAt;

    QMetaObject::invokeMethod(text,
                              "linkAt",
                              Qt::DirectConnection,
                              Q_RETURN_ARG(QString, linkAt),
                              Q_ARG(qreal, pos.x()),
                              Q_ARG(qreal, pos.y()));

    if (linkAt.isEmpty()) {
        return nullptr;
    }

    int childId = linkAt.toInt();
    if (childId < 0 || childId >= text->childItems().length()) {
        return nullptr;
    }

    return text->childItems().at(childId);
}

bool pointerEventsBlocked(QQuickItem* item) {
    QQuickItem* parent = item;
    while (parent) {
        QVariant pointerEvents = parent->property("p_pointerEvents");
        if (pointerEvents.isValid() && pointerEvents.toString() == "none") {
            return true;
        }

        parent = parent->parentItem();
    }

    return false;
}

QVariantMap makeReactTouchEvent(QQuickItem* item, QMouseEvent* event) {

    const QPointF& lp = event->localPos();

    // Find the deepest match
    QQuickItem* target = nullptr;
    QQuickItem* next = item;
    QPointF local = lp;
    forever {
        target = next;

        if (pointerEventsBlocked(target)) {
            return QVariantMap{};
        }

        QString className(target->metaObject()->className());

        if (className.startsWith("ReactScrollListView")) {
            next = getChildFromScrollView(target, local);
        } else if (className.startsWith("ReactText")) {
            QQuickItem* child = getChildFromText(target, local);
            if (child) {
                target = child;
            }
            break;
        } else {
            next = target->childAt(local.x(), local.y());
        }

        if (next == nullptr || !next->isEnabled()) {
            break;
        }
        local = target->mapToItem(next, local);
    }

    AttachedProperties* ap = AttachedProperties::get(target, false);
    if (ap == nullptr) {
        qWarning() << __PRETTY_FUNCTION__ << "target was not a reactItem";
        return QVariantMap{};
    }
    QString button;
    if (event->button() & Qt::LeftButton)
        button = "left";
    else if (event->button() & Qt::RightButton)
        button = "right";

    return createTouchArgs(ap->tag(), lp, local, button, event->timestamp());
}

} // namespace utilities
