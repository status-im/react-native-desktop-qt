
#ifndef REACTPIPEEXECUTOR_H
#define REACTPIPEEXECUTOR_H

#include <QVariant>

class QProcess;

class ReactPipeExecutor : public QObject
{
  Q_OBJECT

public:
  ReactPipeExecutor(QObject* parent);
  ~ReactPipeExecutor();

  void init();

  void injectJson(const QString& name, const QString& json);
  void executeApplicationScript(const QByteArray& script, const QUrl& sourceUrl);
  void executeJSCall(const QString& moduleDotMethod, const QVariantList& args = QVariantList());

Q_SIGNALS:
  void applicationScriptDone();
  void javascriptCallDone(const QVariant& result);

private Q_SLOTS:
  void readReply();

private:
  void sendRequest(const QByteArray& request);
  void handleResponse();
  void handleExecuteApplicationScriptResponse(const QVariantMap& message);
  void handleExecuteJavascriptCallResponse(const QVariantMap& message);

  QVariantMap injectedObjects;
  QProcess* m_nodeProcess;

  QByteArray m_inputBuffer;
};


#endif // REACTPIPEEXECUTOR_H
