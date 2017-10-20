
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

#include <functional>

#include <QByteArray>
#include <QIODevice>
#include <QObject>
#include <QQueue>
#include <QStateMachine>
#include <QVariant>

#include "serverconnection.h"

class ExecutorPrivate;
class Executor : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(Executor)

signals:
    void commandReceived(int size);

public:
    typedef std::function<void(const QJsonDocument&)> ExecuteCallback;

    Executor(ServerConnection* conn, QObject* parent = nullptr);
    ~Executor();

    virtual void init();

    virtual void injectJson(const QString& name, const QVariant& data);
    virtual void executeApplicationScript(const QByteArray& script, const QUrl& sourceUrl);
    virtual void executeJSCall(const QString& method,
                               const QVariantList& args = QVariantList(),
                               const ExecuteCallback& callback = ExecuteCallback());

Q_SIGNALS:
    // TODO: KOZIEIEV: remove from Executor. Maybe in Bridge. Executor shouldn't know about app-related things.
    void applicationScriptDone();

private:
    QScopedPointer<ExecutorPrivate> d_ptr;
};

#endif // EXECUTOR_H
