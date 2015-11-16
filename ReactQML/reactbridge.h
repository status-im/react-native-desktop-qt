#ifndef REACTBRIDGE_H
#define REACTBRIDGE_H

#include <QMap>
#include <QUrl>
#include <QObject>


class QQuickItem;
class QQmlEngine;
class QNetworkAccessManager;
class ReactNetExecutor;
class ReactModuleData;

class ReactBridge : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QQuickItem* visualParent READ visualParent WRITE setVisualParent)
  Q_PROPERTY(QQmlEngine* qmlEngine READ qmlEngine WRITE setQmlEngine)
  Q_PROPERTY(QNetworkAccessManager* networkAccessManager READ networkAccessManager WRITE setNetworkAccessManager)
  Q_PROPERTY(QUrl bundleUrl READ bundleUrl WRITE setBundleUrl)
  Q_PROPERTY(QList<ReactModuleData*> modules READ modules);

  enum Fields { FieldRequestModuleIDs, FieldMethodIDs, FieldParams };

public:
  ReactBridge(QObject* parent = 0);
  ~ReactBridge();

  void init();

  void enqueueJSCall(const QString& module, const QString& method, const QVariantList& args);
  void executeSourceCode(const QByteArray& sourceCode);

  QQuickItem* visualParent() const;
  void setVisualParent(QQuickItem* item);

  QQmlEngine* qmlEngine() const;
  void setQmlEngine(QQmlEngine* qmlEngine);

  QNetworkAccessManager* networkAccessManager() const;
  void setNetworkAccessManager(QNetworkAccessManager* nam);

  QUrl bundleUrl() const;
  void setBundleUrl(const QUrl& bundleUrl);

  QList<ReactModuleData*> modules() const;

Q_SIGNALS:
  void bridgeReady();

private Q_SLOTS:
  void sourcesFinished();
  void applicationScriptDone();

private:
  void loadSource();
  void initModules();
  void processResult(const QJsonDocument& document);

  ReactNetExecutor* m_executor;
  QQmlEngine* m_qmlEngine;
  QQuickItem* m_visualParent;
  QNetworkAccessManager* m_nam;
  QByteArray m_sourceCode;
  QUrl m_bundleUrl;
  QMap<int, ReactModuleData*> m_modules;
};

#endif // REACTBRIDGE_H
