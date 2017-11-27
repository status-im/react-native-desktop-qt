import QtQuick 2.9
import QtQuick.Controls 2.2

import QtWebView 1.1
import React 0.1 as React

WebView {
    id: webViewRoot
    property var p_source: ''
    property var webViewManager: null
    property var flexbox: React.Flexbox {control: webViewRoot}
    url: (p_source && p_source.uri) ? p_source.uri : ""

    onWebViewManagerChanged: {
        if (!webViewManager) {
            return;
        }
        webViewManager.s_reload.connect(webViewRoot.reload);
        webViewManager.s_goBack.connect(webViewRoot.goBack);
        webViewManager.s_goForward.connect(webViewRoot.goForward);
    }
}
