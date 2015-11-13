
#ifndef REACTNETEXECUTOR_H
#define REACTNETEXECUTOR_H

#include <functional>

#include <QVariant>

class QTcpSocket;

class ReactNetExecutor : public QObject
{
  Q_OBJECT

  typedef std::function<void(const QJsonDocument&)> ExecuteCallback;

public:
  ReactNetExecutor(QObject* parent);
  ~ReactNetExecutor();

  void init();

  void injectJson(const QString& name, const QVariant& data);
  void executeApplicationScript(const QByteArray& script, const QUrl& sourceUrl);
  void executeJSCall(const QString& module,
                     const QString& method,
                     const QVariantList& args = QVariantList(),
                     const ExecuteCallback& callback = ExecuteCallback()
                     );

Q_SIGNALS:
  void applicationScriptDone();
  void javascriptCallDone(const QVariant& result);

private Q_SLOTS:
  void readReply();

private:
  void sendRequest(const QByteArray& request);
  QJsonDocument handleResponse();
  void handleExecuteApplicationScriptResponse(const QVariantMap& message);
  void handleExecuteJavascriptCallResponse(const QVariantMap& message);

  QTcpSocket* m_socket;

  QByteArray m_inputBuffer;
};


#endif // REACTNETEXECUTOR_H
