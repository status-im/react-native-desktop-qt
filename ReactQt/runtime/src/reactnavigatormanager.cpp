
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

#include "reactattachedproperties.h"
#include "reactbridge.h"
#include "reactnavigatormanager.h"
#include "reactpropertyhandler.h"
#include "reactuimanager.h"
#include "utilities.h"

using namespace utilities;

void ReactNavigatorManager::push(int containerTag, int viewTag) {
    QQuickItem* container = bridge()->uiManager()->viewForTag(containerTag);
    QQuickItem* view = bridge()->uiManager()->viewForTag(viewTag);
    if (container == nullptr || view == nullptr) {
        qCritical() << __PRETTY_FUNCTION__ << "Invalid react tag passed";
        return;
    }

    invokeMethod("push(QVariant)", container, QVariantList{QVariant::fromValue(view)});
}

void ReactNavigatorManager::pop(int containerTag) {
    QQuickItem* container = bridge()->uiManager()->viewForTag(containerTag);
    if (container == nullptr) {
        qCritical() << __PRETTY_FUNCTION__ << "Invalid react tag passed";
        return;
    }
    invokeMethod("pop()", container);
}

void ReactNavigatorManager::clear(int containerTag) {
    QQuickItem* container = bridge()->uiManager()->viewForTag(containerTag);
    if (container == nullptr) {
        qCritical() << __PRETTY_FUNCTION__ << "Invalid react tag passed";
        return;
    }
    invokeMethod("clear()", container);
}

ReactNavigatorManager::ReactNavigatorManager(QObject* parent) : ReactViewManager(parent), m_id(0) {}

ReactNavigatorManager::~ReactNavigatorManager() {}

ReactViewManager* ReactNavigatorManager::viewManager() {
    return this;
}

QString ReactNavigatorManager::moduleName() {
    return "RCTNavigatorManager";
}

QStringList ReactNavigatorManager::customBubblingEventTypes() {
    return QStringList{normalizeInputEventName("onBackButtonPress")};
}

void ReactNavigatorManager::backTriggered() {
    QQuickItem* viewItem = qobject_cast<QQuickItem*>(sender());

    bool backButtonPressFlagSet = viewItem->property("p_onBackButtonPress").toBool();

    if (backButtonPressFlagSet) {
        notifyJsAboutEvent(tag(viewItem), "onBackButtonPress", {});
    }
}

void ReactNavigatorManager::configureView(QQuickItem* view) const {
    ReactViewManager::configureView(view);
    connect(view, SIGNAL(backTriggered()), SLOT(backTriggered()));
}

QString ReactNavigatorManager::qmlComponentFile() const {
    return ":/qml/ReactNavigator.qml";
}

#define _R_ARG(argn) QGenericArgument(argn.typeName(), argn.data())
void ReactNavigatorManager::invokeMethod(const QString& methodSignature, QQuickItem* item, const QVariantList& args) {
    QMetaMethod m = findMethod(methodSignature, item);
    if (!m.isValid()) {
        qCritical() << __PRETTY_FUNCTION__ << "Attempting to invoke unknown method";
        return;
    }

    // XXX: share this machinery somehow
    switch (args.size()) {
    case 0:
        m.invoke(item, Qt::DirectConnection);
        break;
    case 1:
        m.invoke(item, Qt::DirectConnection, _R_ARG(args.at(0)));
        break;
    }
}

QMetaMethod ReactNavigatorManager::findMethod(const QString& methodSignature, QQuickItem* item) {
    QPair<QString, QQuickItem*> mp = qMakePair(methodSignature, item);
    if (m_methodCache.find(mp) != m_methodCache.end()) {
        return m_methodCache.value(mp);
    } else {
        const QMetaObject* metaObject = item->metaObject();
        const int methodCount = metaObject->methodCount();

        for (int i = metaObject->methodOffset(); i < methodCount; ++i) {
            QMetaMethod m = metaObject->method(i);
            if (m.methodSignature() == methodSignature.toLocal8Bit()) {
                m_methodCache.insert(qMakePair(methodSignature, item), m);
                return m;
            }
        }
    }

    return QMetaMethod();
}

#include "reactnavigatormanager.moc"
