
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

#include <QString>
#include <QUrl>
#include <QQueue>
#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QStateMachine>

#include <QDebug>

#include "reactnetexecutor.h"


namespace {
struct RegisterClass {
  RegisterClass() { qRegisterMetaType<ReactNetExecutor*>(); }
} registerClass;
}

class ReactNetExecutorPrivate : public QObject {
  Q_OBJECT
public:
  QString serverHost = "localhost";
  QTcpSocket* socket = nullptr;
  QStateMachine* machina = nullptr;
  QByteArray inputBuffer;
  QQueue<QByteArray> requestQueue;
  QQueue<ReactExecutor::ExecuteCallback> responseQueue;

  void processRequests() {
    if (socket->state() != QAbstractSocket::ConnectedState ||
        requestQueue.isEmpty()) {
      return;
    }

    QByteArray request = requestQueue.dequeue();
    quint32 length = request.size();
    socket->write((const char*)&length, sizeof(length));
    socket->write(request.constData(), request.size());
    socket->flush();
  }

public Q_SLOTS:
  void readReply() {
    if (inputBuffer.capacity() == 0) {
      quint32 length = 0;
      if (socket->bytesAvailable() < sizeof(length))
        return;
      socket->read((char*)&length, sizeof(length));
      inputBuffer.reserve(length);
    }

    inputBuffer += socket->read(inputBuffer.capacity() - inputBuffer.size());

    if (inputBuffer.size() < inputBuffer.capacity())
      return;

    ReactExecutor::ExecuteCallback callback = responseQueue.dequeue();
    if (callback) {
      QJsonDocument doc;
      if (inputBuffer != "undefined") {
        doc = QJsonDocument::fromJson(inputBuffer);
      }
      callback(doc);
    }

    inputBuffer.clear();
  }
};


ReactNetExecutor::ReactNetExecutor(QObject* parent)
  : ReactExecutor(parent)
  , d_ptr(new ReactNetExecutorPrivate)
{
  Q_D(ReactNetExecutor);
  QString serverHost = qgetenv("REACT_SERVER_HOST");
  if (!serverHost.isEmpty())
    d->serverHost = serverHost;

  d->socket = new QTcpSocket(this);
  connect(d->socket, SIGNAL(readyRead()), d, SLOT(readReply()));

  d->machina = new QStateMachine(this);

  QState* initialState = new QState();
  QState* errorState = new QState();
  QState* readyState = new QState();

  initialState->addTransition(d->socket, SIGNAL(connected()), readyState);
  readyState->addTransition(d->socket, SIGNAL(error(QAbstractSocket::SocketError)), errorState);
  readyState->addTransition(d->socket, SIGNAL(disconnected()), errorState);

  connect(initialState, &QAbstractState::entered, [=] {
      d->socket->connectToHost(d->serverHost, 5000);
    });
  connect(readyState, &QAbstractState::entered, [=] {
      d->processRequests();
    });
  connect(errorState, &QAbstractState::entered, [=] {
      d->machina->stop();
    });

  d->machina->addState(initialState);
  d->machina->addState(errorState);
  d->machina->addState(readyState);
  d->machina->setInitialState(initialState);
}

ReactNetExecutor::~ReactNetExecutor()
{
}

QString ReactNetExecutor::serverHost() const
{
  return d_func()->serverHost;
}

void ReactNetExecutor::setServerHost(const QString& serverHost)
{
  Q_D(ReactNetExecutor);
  if (d->serverHost == serverHost)
    return;
  d->serverHost = serverHost;
}

void ReactNetExecutor::init()
{
  d_func()->machina->start();
}

void ReactNetExecutor::injectJson(const QString& name, const QVariant& data)
{
  QJsonDocument doc = QJsonDocument::fromVariant(data);
  processRequest(name.toLocal8Bit() + "=" + doc.toJson(QJsonDocument::Compact));
}

void ReactNetExecutor::executeApplicationScript(const QByteArray& script, const QUrl& /*sourceUrl*/)
{
  processRequest(script, [=](const QJsonDocument&) {
      Q_EMIT applicationScriptDone();
    });
}

void ReactNetExecutor::executeJSCall(
  const QString& method,
  const QVariantList& args,
  const ReactExecutor::ExecuteCallback& callback
) {
  QByteArrayList stringifiedArgs;
  for (const QVariant& arg : args) {
    if (arg.type() == QVariant::List || arg.type() == QVariant::Map) {
      QJsonDocument doc = QJsonDocument::fromVariant(arg);
      stringifiedArgs += doc.toJson(QJsonDocument::Compact);
    } else {
      stringifiedArgs += '"' + arg.toString().toLocal8Bit() + '"';
    }
  }

  processRequest(
     QByteArray("__fbBatchedBridge.") + method.toLocal8Bit() + "(" + stringifiedArgs.join(',') + ");",
     callback);
}

void ReactNetExecutor::processRequest(
  const QByteArray& request,
  const ReactExecutor::ExecuteCallback& callback
) {
  Q_D(ReactNetExecutor);

  d->requestQueue.enqueue(request);
  d->responseQueue.enqueue(callback);
  d->processRequests();
}

#include "reactnetexecutor.moc"

