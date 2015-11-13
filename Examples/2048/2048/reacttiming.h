#ifndef REACTTIMING_H
#define REACTTIMING_H

#include <QMap>
#include <QVariant>
#include <QObject>
#include <QPointer>
#include <QTimer>

#include "reactmoduleinterface.h"

class ReactBridge;

class ReactTiming
  : public QObject
  , public ReactModuleInterface
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

public:
  ReactTiming(QObject* parent = 0);
  ~ReactTiming();

  void setBridge(ReactBridge* bridge);

  // TODO: this doesnt seem right
  UbuntuViewManager* viewManager();

  QString moduleName();
  QStringList methodsToExport();
  QVariantMap constantsToExport();

  Q_INVOKABLE void createTimer(int callbackId, int duration, double jsSchedulingTime, bool repeats);
  Q_INVOKABLE void deleteTimer(int timerId);

private:
  QPointer<ReactBridge> m_bridge;
  QMap<int, QTimer*> m_activeTimers;
};

#endif // REACTTIMING_H
