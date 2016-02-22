#ifndef REACTBRIDGE_H
#define REACTBRIDGE_H

#include <QUrl>
#include <QObject>
#include <QScopedPointer>

class QQuickItem;
class QQmlEngine;
class QNetworkAccessManager;
class ReactModuleData;
class ReactUIManager;


class ReactBridgePrivate;
class ReactBridge : public QObject
{
  Q_OBJECT
  Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged);
  Q_PROPERTY(QQuickItem* visualParent READ visualParent WRITE setVisualParent)
  Q_PROPERTY(QQmlEngine* qmlEngine READ qmlEngine WRITE setQmlEngine)
  Q_PROPERTY(QNetworkAccessManager* networkAccessManager READ networkAccessManager WRITE setNetworkAccessManager)
  Q_PROPERTY(QUrl bundleUrl READ bundleUrl WRITE setBundleUrl)
  Q_PROPERTY(QList<ReactModuleData*> modules READ modules)
  Q_PROPERTY(ReactUIManager* uiManager READ uiManager)

  enum Fields { FieldRequestModuleIDs, FieldMethodIDs, FieldParams };

  Q_DECLARE_PRIVATE(ReactBridge)

public:
  ReactBridge(QObject* parent = 0);
  ~ReactBridge();

  void init();
  void reload();

  void enqueueJSCall(const QString& module, const QString& method, const QVariantList& args);
  void invokeAndProcess(const QString& module, const QString& method, const QVariantList &args);
  void executeSourceCode(const QByteArray& sourceCode);

  // XXX: maybe rename
  bool ready() const;
  void setReady(bool ready);

  QQuickItem* visualParent() const;
  void setVisualParent(QQuickItem* item);

  QQmlEngine* qmlEngine() const;
  void setQmlEngine(QQmlEngine* qmlEngine);

  QNetworkAccessManager* networkAccessManager() const;
  void setNetworkAccessManager(QNetworkAccessManager* nam);

  QUrl bundleUrl() const;
  void setBundleUrl(const QUrl& bundleUrl);

  QList<ReactModuleData*> modules() const;
  ReactUIManager* uiManager() const;

Q_SIGNALS:
  void readyChanged();

private Q_SLOTS:
  void sourcesFinished();
  void applicationScriptDone();

private:
  void loadSource();
  void initModules();
  void injectModules();
  void processResult(const QJsonDocument& document);

  QScopedPointer<ReactBridgePrivate> d_ptr;
};

#endif // REACTBRIDGE_H
