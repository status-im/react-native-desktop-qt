
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef JSWEBENGINEEXECUTOR_H
#define JSWEBENGINEEXECUTOR_H

#include <QByteArray>
#include <QIODevice>
#include <QQueue>
#include <QStateMachine>

#include "ijsexecutor.h"

class JSWebEngineExecutorPrivate;
class JSWebEngineExecutor : public IJsExecutor {
    Q_OBJECT
    Q_DECLARE_PRIVATE(JSWebEngineExecutor)

public:
    JSWebEngineExecutor(QObject* parent = nullptr);
    ~JSWebEngineExecutor();

    Q_INVOKABLE virtual void init();
    Q_INVOKABLE virtual void resetConnection();

    Q_INVOKABLE virtual void injectJson(const QString& name, const QVariant& data);
    Q_INVOKABLE virtual void executeApplicationScript(const QByteArray& script, const QUrl& sourceUrl);
    Q_INVOKABLE virtual void executeJSCall(const QString& method,
                                           const QVariantList& args = QVariantList(),
                                           const IJsExecutor::ExecuteCallback& callback = ExecuteCallback());

private:
    QScopedPointer<JSWebEngineExecutorPrivate> d_ptr;
};

#endif // JSWEBENGINEEXECUTOR_H
