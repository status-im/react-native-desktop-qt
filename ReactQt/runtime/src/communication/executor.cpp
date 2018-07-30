
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

#include "executor.h"
#include <QJsonDocument>
#include <QSharedPointer>

class ExecutorPrivate : public QObject {
public:
    ExecutorPrivate(Executor* e) : QObject(e), q_ptr(e) {}

    virtual ServerConnection* connection();
    void processRequests();
    bool readPackageHeaderAndAllocateBuffer();
    bool readPackageBodyToBuffer();
    void processRequest(const QByteArray& request,
                        const Executor::ExecuteCallback& callback = Executor::ExecuteCallback());

public slots:
    void readReply();
    bool readCommand();
    void passReceivedDataToCallback(const QByteArray& data);
    void setupStateMachine();

public:
    QQueue<QByteArray> m_requestQueue;
    QQueue<Executor::ExecuteCallback> m_responseQueue;
    QStateMachine* m_machina = nullptr;
    QByteArray m_inputBuffer;
    QSharedPointer<ServerConnection> m_connection = nullptr;
    Executor* q_ptr = nullptr;
};

Executor::Executor(ServerConnection* conn, QObject* parent) : IExecutor(parent), d_ptr(new ExecutorPrivate(this)) {
    Q_ASSERT(conn);
    Q_D(Executor);
    d->m_connection = QSharedPointer<ServerConnection>(conn);
    connect(d->connection(), &ServerConnection::dataReady, d, &ExecutorPrivate::readReply);

    qRegisterMetaType<Executor::ExecuteCallback>();
}

Executor::~Executor() {
    resetConnection();
}

void ExecutorPrivate::setupStateMachine() {
    m_machina = new QStateMachine(this);

    QState* initialState = new QState();
    QState* errorState = new QState();
    QState* readyState = new QState();

    initialState->addTransition(connection(), SIGNAL(connectionReady()), readyState);
    readyState->addTransition(connection(), SIGNAL(connectionError()), errorState);

    connect(initialState, &QAbstractState::entered, [=] { connection()->openConnection(); });
    connect(readyState, &QAbstractState::entered, [=] { processRequests(); });
    connect(errorState, &QAbstractState::entered, [=] { m_machina->stop(); });

    m_machina->addState(initialState);
    m_machina->addState(errorState);
    m_machina->addState(readyState);
    m_machina->setInitialState(initialState);
}

void Executor::injectJson(const QString& name, const QVariant& data) {
    QJsonDocument doc = QJsonDocument::fromVariant(data);
    d_ptr->processRequest(name.toLocal8Bit() + "=" + doc.toJson(QJsonDocument::Compact) + ";");
}

void Executor::executeApplicationScript(const QByteArray& script, const QUrl& /*sourceUrl*/) {
    d_ptr->processRequest(script, [=](const QJsonDocument&) { Q_EMIT applicationScriptDone(); });
}

void Executor::executeJSCall(const QString& method,
                             const QVariantList& args,
                             const Executor::ExecuteCallback& callback) {

    QByteArrayList stringifiedArgs;
    for (const QVariant& arg : args) {
        if (arg.type() == QVariant::List || arg.type() == QVariant::Map) {
            QJsonDocument doc = QJsonDocument::fromVariant(arg);
            stringifiedArgs += doc.toJson(QJsonDocument::Compact);
        } else {
            stringifiedArgs += '"' + arg.toString().toLocal8Bit() + '"';
        }
    }

    d_ptr->processRequest(
        QByteArray("__fbBatchedBridge.") + method.toLocal8Bit() + "(" + stringifiedArgs.join(',') + ");", callback);
}

ServerConnection* ExecutorPrivate::connection() {
    Q_ASSERT(m_connection);
    return m_connection.data();
}

void Executor::init() {
    d_ptr->setupStateMachine();
    d_ptr->m_machina->start();
}

void Executor::resetConnection() {
    d_ptr->connection()->device()->close();
}

void ExecutorPrivate::processRequests() {
    if (!connection()->isReady() || m_requestQueue.isEmpty()) {
        return;
    }

    QByteArray request = m_requestQueue.dequeue();
    quint32 length = request.size();
    connection()->device()->write((const char*)&length, sizeof(length));
    connection()->device()->write(request.constData(), request.size());
}

bool ExecutorPrivate::readPackageHeaderAndAllocateBuffer() {
    if (m_inputBuffer.capacity() == 0) {
        quint32 length = 0;
        if (connection()->device()->bytesAvailable() < sizeof(length)) {
            return false;
        }
        connection()->device()->read((char*)&length, sizeof(length));
        m_inputBuffer.reserve(length);
    }
    return true;
}

bool ExecutorPrivate::readPackageBodyToBuffer() {
    int toRead = m_inputBuffer.capacity() - m_inputBuffer.size();
    QByteArray read = connection()->device()->read(toRead);
    m_inputBuffer += read;

    if (m_inputBuffer.size() < m_inputBuffer.capacity())
        return false;

    return true;
}

void ExecutorPrivate::readReply() {
    while (readCommand()) {
        ;
    }
}

bool ExecutorPrivate::readCommand() {

    if (!readPackageHeaderAndAllocateBuffer())
        return false;

    if (!readPackageBodyToBuffer())
        return false;

    q_ptr->commandReceived(m_inputBuffer.length());
    passReceivedDataToCallback(m_inputBuffer);
    m_inputBuffer.clear();
    return true;
}

void ExecutorPrivate::passReceivedDataToCallback(const QByteArray& data) {
    if (m_responseQueue.size()) {
        Executor::ExecuteCallback callback = m_responseQueue.dequeue();
        if (callback) {
            QJsonDocument doc;
            if (data != "undefined") {
                doc = QJsonDocument::fromJson(data);
            }
            callback(doc);
        }
    }
}

void ExecutorPrivate::processRequest(const QByteArray& request, const Executor::ExecuteCallback& callback) {

    m_requestQueue.enqueue(request);
    m_responseQueue.enqueue(callback);
    processRequests();
}
