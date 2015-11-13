
#include <QString>
#include <QUrl>
#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>

#include <QDebug>

#include "reactnetexecutor.h"


ReactNetExecutor::ReactNetExecutor(QObject* parent) : QObject(parent)
  , m_socket(new QTcpSocket(this))
{
  m_socket->connectToHost("localhost", 5000);
  //  connect(m_socket, SIGNAL(connected()), SLOT(connected()));
  //  connect(m_socket, SIGNAL(disconnected()), SLOT(disconnected()));
  connect(m_socket, SIGNAL(readyRead()), SLOT(readReply()));
}

ReactNetExecutor::~ReactNetExecutor()
{
}

void ReactNetExecutor::init()
{
  // TODO: move socket connect to here
}

void ReactNetExecutor::injectJson(const QString& name, const QVariant& data)
{
  qDebug() << __func__ << "name=" << name; // << "json=" << data;

  QJsonDocument doc = QJsonDocument::fromVariant(data);

  sendRequest(name.toLocal8Bit() + "=" + doc.toJson(QJsonDocument::Compact));
  handleResponse();
}

void ReactNetExecutor::executeApplicationScript(const QByteArray& script, const QUrl& /*sourceUrl*/)
{
  qDebug() << __func__;

  // Size is a problem?
  sendRequest(script);

  handleResponse();

  Q_EMIT applicationScriptDone();
}

void ReactNetExecutor::executeJSCall(
    const QString& module,
    const QString& method,
    const QVariantList& args,
    const ExecuteCallback& callback
) {
  qDebug() << __func__ << "method=" << method << "args=" << args;

  m_socket->waitForConnected();  // TODO:

  QByteArrayList stringifiedArgs;
  Q_FOREACH(const QVariant& arg, args) {
    if (arg.type() == QVariant::List || arg.type() == QVariant::Map) {
      QJsonDocument doc = QJsonDocument::fromVariant(arg);
      stringifiedArgs += doc.toJson(QJsonDocument::Compact);
    } else {
      stringifiedArgs += '"' + arg.toString().toLocal8Bit() + '"';
    }
  }

  sendRequest(
        QByteArray("require(\"") + module.toLocal8Bit() + "\")." +
        method.toLocal8Bit() + "(" + stringifiedArgs.join(',') + ");"
      );

  QJsonDocument result = handleResponse();
  if (callback)
    callback(result);
}

void ReactNetExecutor::readReply()
{
  /*
  if (m_inputBuffer.capacity() == 0) {
    quint32 length = 0;
    if (m_nodeProcess->bytesAvailable() < sizeof(length)) {
      return;
    }
    m_nodeProcess->read((char*)&length, sizeof(length));
    m_inputBuffer.reserve(length);
  }

  if (m_nodeProcess->bytesAvailable() < m_inputBuffer.capacity()) {
    return;
  }

  m_inputBuffer += m_nodeProcess->read(m_inputBuffer.capacity());
  qDebug() << __func__ << "collected=" << m_inputBuffer;
  */
}

void ReactNetExecutor::sendRequest(const QByteArray& request)
{
  m_socket->waitForConnected();  // TODO:

  quint32 length = request.size();
  m_socket->write((const char*)&length, sizeof(length));
  m_socket->write(request.constData(), request.size());
}

QJsonDocument ReactNetExecutor::handleResponse()
{
  m_socket->waitForReadyRead();

  QJsonDocument doc;
  // Read JSON response
  QByteArray response = m_socket->readAll();
  if (response == "undefined")
    return doc;
  doc = QJsonDocument::fromJson(response);
  return doc;
}

/*
void ReactNetExecutor::handleExecuteApplicationScriptResponse
(
  const QVariantMap& message
) {
  Q_EMIT applicationScriptDone();
}

void ReactNetExecutor::handleExecuteJavascriptCallResponse
(
  const QVariantMap& message
) {
  Q_EMIT javascriptCallDone(message.toVariant());
}
*/
