
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

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <QByteArray>
#include <QIODevice>
#include <QQueue>
#include <QStateMachine>

#include "iexecutor.h"
#include "serverconnection.h"

class ExecutorPrivate;
class Executor : public IExecutor {
    Q_OBJECT
    Q_DECLARE_PRIVATE(Executor)

public:
    typedef std::function<void(const QJsonDocument&)> ExecuteCallback;

    Executor(ServerConnection* conn, QObject* parent = nullptr);
    ~Executor();

    virtual void init();
    virtual void resetConnection();

    virtual void injectJson(const QString& name, const QVariant& data);
    virtual void executeApplicationScript(const QByteArray& script, const QUrl& sourceUrl);
    virtual void executeJSCall(const QString& method,
                               const QVariantList& args = QVariantList(),
                               const ExecuteCallback& callback = ExecuteCallback());

private:
    QScopedPointer<ExecutorPrivate> d_ptr;
};

#endif // EXECUTOR_H
