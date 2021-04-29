
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <QObject>
#include <QProcess>
#include <QTcpSocket>

class ServerConnection : public QObject {
    Q_OBJECT

signals:
    void dataReady();
    void connectionReady();
    void connectionError();

public:
    ServerConnection(QObject* parent = nullptr);
    virtual ~ServerConnection() {}

    virtual void openConnection() = 0;
    virtual bool isReady() = 0;

public:
    virtual QIODevice* device() = 0;
};

class LocalServerConnection : public ServerConnection {
    Q_OBJECT
public:
    Q_INVOKABLE LocalServerConnection(QObject* parent = nullptr);

    virtual void openConnection() override;
    virtual bool isReady() override;
    bool logErrors() const;
    void setLogErrors(bool logErrors);

private:
    virtual QIODevice* device() override;

private:
    bool m_logErrors = true;
    QProcess* nodeProcess = nullptr;
};

class RemoteServerConnection : public ServerConnection {
    Q_OBJECT
public:
    Q_INVOKABLE RemoteServerConnection(QObject* parent = nullptr);
    QString serverHost() const;
    void setServerHost(const QString& serverHost);
    void setPort(int m_port);

    virtual void openConnection() override;
    virtual bool isReady() override;

private:
    virtual QIODevice* device() override;

private:
    QString m_serverHost = "127.0.0.1";
    int m_port = 5000;
    QTcpSocket* m_socket = nullptr;
};

#endif // SERVERCONNECTION_H
