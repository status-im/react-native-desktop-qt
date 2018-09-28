
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "serverconnection.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

namespace {
QString default_executable{"node"};
QString default_script{"ubuntu-server.js"};
QStringList default_arguments{"--pipe"};

QString itemWithPath(const QString& item, const QStringList& searchPaths) {
    const QStringList local_paths{".", "bin"};
    for (const QString& searchPath : searchPaths) {
        for (const QString& localPath : local_paths) {
            QString testPath = searchPath + "/" + localPath + "/";
            if (QFileInfo::exists(testPath + item))
                return testPath + item;
        }
    }
    return item;
}
} // namespace

namespace {
struct RegisterLocal {
    RegisterLocal() {
        qRegisterMetaType<LocalServerConnection*>();
    }
} registerLocal;

struct RegisterRemote {
    RegisterRemote() {
        qRegisterMetaType<RemoteServerConnection*>();
    }
} registerRemote;
} // namespace

ServerConnection::ServerConnection(QObject* parent) : QObject(parent) {}

LocalServerConnection::LocalServerConnection(QObject* parent)
    : ServerConnection(parent)

{
    // applicationDirPath needs to be called after creation of Application
    const QStringList search_paths{QDir::currentPath(), QCoreApplication::applicationDirPath()};

    nodeProcess = new QProcess(this);
    QString program = itemWithPath(default_executable, search_paths);
    QStringList args = QStringList() << itemWithPath(default_script, search_paths) << default_arguments;

    nodeProcess->setProgram(program);
    nodeProcess->setArguments(args);

    connect(nodeProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError) {
        qDebug() << "Ubuntu process error: " << nodeProcess->errorString();
    });

    connect(nodeProcess, &QProcess::readyReadStandardOutput, this, &LocalServerConnection::dataReady);
    connect(nodeProcess, &QProcess::readyReadStandardError, [=] {
        if (m_logErrors) {
            qWarning() << "Report from co-process: \"\"\"";
            qWarning().noquote() << nodeProcess->readAllStandardError().trimmed();
            qWarning() << "\"\"\"";
        }
    });

    connect(nodeProcess, &QProcess::started, [=]() { emit connectionReady(); });
    connect(nodeProcess,
            static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::error),
            [=](QProcess::ProcessError) { emit connectionError(); });
}

QIODevice* LocalServerConnection::device() {
    Q_ASSERT(nodeProcess);
    return nodeProcess;
}

void LocalServerConnection::openConnection() {
    nodeProcess->start();
}

bool LocalServerConnection::isReady() {
    return nodeProcess->state() == QProcess::Running;
}

bool LocalServerConnection::logErrors() const {
    return m_logErrors;
}

void LocalServerConnection::setLogErrors(bool logErrors) {
    m_logErrors = logErrors;
}

RemoteServerConnection::RemoteServerConnection(QObject* parent) : ServerConnection(parent) {
    QString serverHost = qgetenv("REACT_SERVER_HOST");
    if (!serverHost.isEmpty()) {
        m_serverHost = serverHost;
    }

    QString serverPort = qgetenv("REACT_SERVER_PORT");
    if (!serverPort.isEmpty()) {
        int port = serverPort.toInt();
        if (port != 0) {
            m_port = port;
        }
    }

    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::readyRead, this, &RemoteServerConnection::dataReady);
    connect(m_socket, &QTcpSocket::connected, this, [=]() { emit connectionReady(); });
    connect(m_socket,
            static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            this,
            [=](QAbstractSocket::SocketError) { emit connectionError(); });
    connect(m_socket, &QAbstractSocket::disconnected, this, [=]() { emit connectionError(); });
}

QIODevice* RemoteServerConnection::device() {
    Q_ASSERT(m_socket);
    return m_socket;
}

QString RemoteServerConnection::serverHost() const {
    return m_serverHost;
}

void RemoteServerConnection::setServerHost(const QString& serverHost) {
    if (m_serverHost == serverHost)
        return;
    m_serverHost = serverHost;
}

void RemoteServerConnection::setPort(int port) {
    Q_ASSERT(port > 0);
    m_port = port;
}

void RemoteServerConnection::openConnection() {
    m_socket->connectToHost(m_serverHost, m_port);
}

bool RemoteServerConnection::isReady() {
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

#include "serverconnection.moc"
