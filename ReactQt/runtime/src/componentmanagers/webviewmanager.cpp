
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

const QString EVENT_ONERROR = "onLoadingError";
const QString EVENT_ONLOADFINISH = "onLoadingFinish";
const QString EVENT_ONLOADSTART = "onLoadingStart";

class WebViewManagerPrivate {};

WebViewManager::WebViewManager(QObject* parent) : ViewManager(parent), d_ptr(new WebViewManagerPrivate) {}

WebViewManager::~WebViewManager() {}

ViewManager* WebViewManager::viewManager() {
    return this;
}

QString WebViewManager::moduleName() {
    return "RCTWebViewViewManager";
}

QString WebViewManager::qmlComponentFile(const QVariantMap& properties) const {
#ifdef USE_QTWEBKIT
    const QString reactWebViewComponent = QStringLiteral("qrc:/qml/ReactQtWebKitWebView.qml");
#else
    const QString reactWebViewComponent = QStringLiteral("qrc:/qml/ReactWebView.qml");
#endif
    return reactWebViewComponent;
}

void WebViewManager::configureView(QQuickItem* view) const {
    ViewManager::configureView(view);
    view->setProperty("webViewManager", QVariant::fromValue((QObject*)this));
}

bool WebViewManager::shouldLayout() const {
    return true;
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

void WebViewManager::injectJavaScript(const QString& javascript) {
    Q_EMIT s_invokeJS(javascript);
}

void WebViewManager::sendOnLoadEndNotificationToJs(QQuickItem* webView) {
    notifyJsAboutEvent(tag(webView), EVENT_ONLOADFINISH, {});
}
void WebViewManager::sendOnLoadStartNotificationToJs(QQuickItem* webView) {
    notifyJsAboutEvent(tag(webView), EVENT_ONLOADSTART, {});
}
void WebViewManager::sendOnErrorNotificationToJs(QQuickItem* webView) {
    notifyJsAboutEvent(tag(webView), EVENT_ONERROR, {});
}

QStringList WebViewManager::customDirectEventTypes() {
    return QStringList{utilities::normalizeInputEventName(EVENT_ONLOADSTART),
                       utilities::normalizeInputEventName(EVENT_ONLOADFINISH),
                       utilities::normalizeInputEventName(EVENT_ONERROR)};
}

#include "webviewmanager.moc"
