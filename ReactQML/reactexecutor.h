#ifndef REACTEXECUTOR_H
#define REACTEXECUTOR_H

#include <functional>

#include <QVariant>
#include <QObject>

class ReactExecutor : public QObject
{
  Q_OBJECT

public:
  typedef std::function<void(const QJsonDocument&)> ExecuteCallback;

  ReactExecutor(QObject* parent);
  ~ReactExecutor();

  virtual void init() = 0;

  virtual void injectJson(const QString& name, const QVariant& data) = 0;
  virtual void executeApplicationScript(const QByteArray& script, const QUrl& sourceUrl) = 0;
  virtual void executeJSCall(const QString& module,
                             const QString& method,
                             const QVariantList& args = QVariantList(),
                             const ExecuteCallback& callback = ExecuteCallback()
                            ) = 0;

Q_SIGNALS:
  void applicationScriptDone();
};

#endif // REACTEXECUTOR_H
