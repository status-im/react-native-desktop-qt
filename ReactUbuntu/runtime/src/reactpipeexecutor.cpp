
#include <QString>
#include <QUrl>
#include <QObject>
#include <QProcess>
#include <QJsonDocument>

#include <QDebug>

#include "reactpipeexecutor.h"


ReactPipeExecutor::ReactPipeExecutor(QObject* parent)
  : ReactExecutor(parent)
  , m_nodeProcess(new QProcess(this))
{
  m_nodeProcess->setProgram("/home/justin/.nvm/versions/node/v4.2.1/bin/node");
  m_nodeProcess->setArguments(QStringList() << "/home/justin/src/react-native/ubuntu-server.js");
  connect(m_nodeProcess, SIGNAL(readyRead()), SLOT(readReply()));
}

ReactPipeExecutor::~ReactPipeExecutor()
{
}

void ReactPipeExecutor::init()
{
}

void ReactPipeExecutor::injectJson(const QString& name, const QVariant& data)
{
  if (m_nodeProcess->state() != QProcess::Running) {
    m_nodeProcess->start();
    if (!m_nodeProcess->waitForStarted()) {
      qCritical() << "Unable to start node process";
    }
  }

  QJsonDocument doc = QJsonDocument::fromVariant(data);

  sendRequest(name.toLocal8Bit() + "=" + doc.toJson(QJsonDocument::Compact) + ";");
  m_nodeProcess->waitForReadyRead();
  m_nodeProcess->readAll();
}

void ReactPipeExecutor::executeApplicationScript(const QByteArray& script, const QUrl& /*sourceUrl*/)
{
  //  qDebug() << __func__ << "script=" << script;
  qDebug() << __func__;
  if (m_nodeProcess->state() != QProcess::Running) {
    m_nodeProcess->start();
    if (!m_nodeProcess->waitForStarted()) {
      qWarning() << "Unable to start node process";
    }
  }

  // Size is a problem?
  // needs to be wrapped in function invocation?
  sendRequest(script);

  m_nodeProcess->waitForReadyRead();
  QByteArray result = m_nodeProcess->readAll();

  qDebug() << __func__ << "result=" << result;
  Q_EMIT applicationScriptDone();
}

void ReactPipeExecutor::executeJSCall
(
 const QString& method,
 const QVariantList& args,
 const ExecuteCallback& callback
 )
{
  if (m_nodeProcess->state() != QProcess::Running) {
    m_nodeProcess->start();
    if (!m_nodeProcess->waitForStarted()) {
      qCritical() << "Unable to start node process";
    }
  }

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
              QByteArray("__fbBatchedBridge.") +
              method.toLocal8Bit() + "(" + stringifiedArgs.join(',') + ");"
              );

  m_nodeProcess->waitForReadyRead();

  QJsonDocument result = handleResponse();
  if (callback)
    callback(result);

}

void ReactPipeExecutor::readReply()
{
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
  //  handleMessage();
}

void ReactPipeExecutor::sendRequest(const QByteArray& request)
{
  quint32 length = request.size();
  m_nodeProcess->write((const char*)&length, sizeof(length));
  m_nodeProcess->write(request.constData(), request.size());
}

QJsonDocument ReactPipeExecutor::handleResponse()
{
  QByteArray response = m_nodeProcess->readAll();
  return QJsonDocument::fromJson(response);
}

/*
void ReactPipeExecutor::handleExecuteApplicationScriptResponse
(
  const QVariantMap& message
) {
  Q_EMIT applicationScriptDone();
}

void ReactPipeExecutor::handleExecuteJavascriptCallResponse
(
  const QVariantMap& message
) {
  Q_EMIT javascriptCallDone(message.toVariant());
}
*/
