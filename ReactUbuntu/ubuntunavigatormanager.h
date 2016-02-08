#ifndef UBUNTUNAVIGATORMANAGER_H
#define UBUNTUNAVIGATORMANAGER_H

#include <QString>
#include <QMap>

#include "reactviewmanager.h"


class QQuickItem;

class UbuntuNavigatorManager : public ReactViewManager
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

  Q_INVOKABLE void push(int containerTag, int viewTag);
  Q_INVOKABLE void pop(int containerTag);
  Q_INVOKABLE void clear(int containerTag);

public:
  UbuntuNavigatorManager(QObject* parent = 0);
  ~UbuntuNavigatorManager();

  void setBridge(ReactBridge* bridge) override;

  ReactViewManager* viewManager() override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

  QQuickItem* view(const QVariantMap& properties) const override;

private:
  void configureView(QQuickItem* view) const;
  void invokeMethod(const QString& methodSignature,
                    QQuickItem* item,
                    const QVariantList& args = QVariantList{});
  QMetaMethod findMethod(const QString& methodSignature, QQuickItem* item);

  mutable int m_id;
  QMap<QPair<QString, QQuickItem*>, QMetaMethod> m_methodCache;
};

#endif // UBUNTUNAVIGATORMANAGER_H
