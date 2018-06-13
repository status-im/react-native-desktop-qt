import QtQuick 2.9
import QtQuick.Controls 2.2

import QtWebView 1.1
import React 0.1 as React

WebView {
    id: webViewRoot
    property var p_source: ''
    property var webViewManager: null
    property var flexbox: React.Flexbox {control: webViewRoot}
    property var p_testID

    onWebViewManagerChanged: {
        if (!webViewManager) {
            return;
        }
        webViewManager.s_reload.connect(webViewRoot.reload);
        webViewManager.s_goBack.connect(webViewRoot.goBack);
        webViewManager.s_goForward.connect(webViewRoot.goForward);
        webViewManager.s_invokeJS.connect(webViewRoot.runJavaScript);
    }

    onP_sourceChanged: {
        webViewRoot.url = (p_source && p_source.uri) ? p_source.uri : "";
        if(p_source && p_source.html) {
            webViewRoot.loadHtml(p_source.html)
        }
    }

    onLoadingChanged: {
        switch(loadRequest.status) {
        case WebView.LoadStartedStatus:
            webViewManager.sendOnLoadStartNotificationToJs(webViewRoot);
            break;
        case WebView.LoadSucceededStatus:
            webViewManager.sendOnLoadEndNotificationToJs(webViewRoot);
            break;
        case WebView.LoadFailedStatus:
            webViewManager.sendOnErrorNotificationToJs(webViewRoot);
            break;
        }
    }
}
