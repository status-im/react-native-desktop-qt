
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <QGuiApplication>
#include <QQmlComponent>
#include <QQmlProperty>
#include <QQuickItem>
#include <QString>
#include <QVariant>

#include <QDebug>

#include "attachedproperties.h"
#include "bridge.h"
#include "modulemethod.h"
#include "propertyhandler.h"
#include "utilities.h"
#include "webviewmanager.h"

class WebViewManagerPrivate {};

WebViewManager::WebViewManager(QObject* parent) : ViewManager(parent), d_ptr(new WebViewManagerPrivate) {}

WebViewManager::~WebViewManager() {}

ViewManager* WebViewManager::viewManager() {
    return this;
}

QString WebViewManager::moduleName() {
    return "RCTWebViewViewManager";
}

QString WebViewManager::qmlComponentFile() const {
    return "qrc:/qml/ReactWebView.qml";
}

void WebViewManager::configureView(QQuickItem* view) const {
    ViewManager::configureView(view);
    view->setProperty("webViewManager", QVariant::fromValue((QObject*)this));
}

bool WebViewManager::shouldLayout() const {
    return true;
}

QList<ModuleMethod*> WebViewManager::methodsToExport() {
    return QList<ModuleMethod*>{};
}

void WebViewManager::reload() {
    Q_EMIT s_reload();
}

void WebViewManager::goBack() {
    Q_EMIT s_goBack();
}

void WebViewManager::goForward() {
    Q_EMIT s_goForward();
}

#include "webviewmanager.moc"
