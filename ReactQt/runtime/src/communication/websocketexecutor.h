
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef WEBSOCKETEXECUTOR_H
#define WEBSOCKETEXECUTOR_H

#ifdef RCT_DEV

#include <functional>

#include <QUrl>

#include "ijsexecutor.h"

class WebSocketExecutorPrivate;
class WebSocketExecutor : public IJsExecutor {
    Q_OBJECT
    Q_DECLARE_PRIVATE(WebSocketExecutor)

public:
    typedef std::function<void(const QJsonDocument&)> ExecuteCallback;

    WebSocketExecutor(const QUrl& url, QObject* parent = nullptr);
    ~WebSocketExecutor();

    virtual void init();
    virtual void resetConnection() {}

    virtual void injectJson(const QString& name, const QVariant& data);
    virtual void executeApplicationScript(const QByteArray& script, const QUrl& sourceUrl);
    virtual void executeJSCall(const QString& method,
                               const QVariantList& args = QVariantList(),
                               const ExecuteCallback& callback = ExecuteCallback());

    void executeApplicationScriptOnSocketReady();

private:
    QScopedPointer<WebSocketExecutorPrivate> d_ptr;
};

#endif // RCT_DEV

#endif // WEBSOCKETEXECUTOR_H
