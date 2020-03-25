
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef JAVASCRIPTCOREEXECUTOR_H
#define JAVASCRIPTCOREEXECUTOR_H

#ifdef JAVASCRIPTCORE_ENABLED

#include <functional>

#include <QUrl>

#include "ijsexecutor.h"

class Bridge;

class JavaScriptCoreExecutorPrivate;
class JavaScriptCoreExecutor : public IExecutor {
    Q_OBJECT
    Q_DECLARE_PRIVATE(JavaScriptCoreExecutor)

public:
    typedef std::function<void(const QJsonDocument&)> ExecuteCallback;

    JavaScriptCoreExecutor(QObject* parent = nullptr);
    ~JavaScriptCoreExecutor();

    virtual void init();

    virtual void injectJson(const QString& name, const QVariant& data);
    virtual void executeApplicationScript(const QByteArray& script, const QUrl& sourceUrl);
    virtual void executeJSCall(const QString& method,
                               const QVariantList& args = QVariantList(),
                               const ExecuteCallback& callback = ExecuteCallback());

    void executeApplicationScriptOnSocketReady();

    void* getJavaScriptContext();
    void executeOnJavaScriptThread(std::function<void()> func);
    void setBridge(Bridge* bridge);

private:
    QScopedPointer<JavaScriptCoreExecutorPrivate> d_ptr;
};

#endif // JAVASCRIPTCORE_ENABLED

#endif // JAVASCRIPTCOREEXECUTOR_H
