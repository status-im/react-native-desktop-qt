
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

#ifndef NETEXECUTOR_H
#define NETEXECUTOR_H

#include "executor.h"
#include <QTcpSocket>

class NetExecutorPrivate;
class NetExecutor : public Executor {
    Q_OBJECT
    Q_PROPERTY(QString serverHost READ serverHost WRITE setServerHost)

    Q_DECLARE_PRIVATE(NetExecutor)

signals:
    void commandReceived(int size);

public:
    Q_INVOKABLE NetExecutor(QObject* parent = 0);
    ~NetExecutor();

    QString serverHost() const;
    void setServerHost(const QString& serverHost);
    void setPort(int port);

    void init() override;

    void injectJson(const QString& name, const QVariant& data) override;
    void executeApplicationScript(const QByteArray& script, const QUrl& sourceUrl) override;
    void executeJSCall(const QString& method,
                       const QVariantList& args = QVariantList(),
                       const ExecuteCallback& callback = ExecuteCallback()) override;

private:
    void processRequest(const QByteArray& request, const ExecuteCallback& callback = ExecuteCallback());

    QScopedPointer<NetExecutorPrivate> d_ptr;
};

#endif // NETEXECUTOR_H
