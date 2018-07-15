
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef IEXECUTOR_H
#define IEXECUTOR_H

#include <functional>

#include <QByteArray>
#include <QObject>
#include <QVariant>

class IExecutor : public QObject {
    Q_OBJECT

public:
    typedef std::function<void(const QJsonDocument&)> ExecuteCallback;

    IExecutor(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~IExecutor() {}

    virtual void init() = 0;
    virtual void resetConnection() = 0;

    virtual void injectJson(const QString& name, const QVariant& data) = 0;
    virtual void executeApplicationScript(const QByteArray& script, const QUrl& sourceUrl) = 0;
    virtual void executeJSCall(const QString& method,
                               const QVariantList& args = QVariantList(),
                               const ExecuteCallback& callback = ExecuteCallback()) = 0;

Q_SIGNALS:
    // TODO: KOZIEIEV: remove from Executor. Maybe in Bridge. Executor shouldn't know about app-related things.
    void applicationScriptDone();

    void commandReceived(int size);
};

#endif // IEXECUTOR_H
