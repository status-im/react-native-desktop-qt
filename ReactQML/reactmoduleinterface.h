#ifndef REACTMODULEINTERFACE_H
#define REACTMODULEINTERFACE_H

#include <QtPlugin>

class ReactBridge;
class UbuntuViewManager;


class ReactModuleInterface
{
public:
  virtual ~ReactModuleInterface() {}

  virtual void setBridge(ReactBridge* bridge) { Q_UNUSED(bridge); }

  // TODO: this doesnt seem right
  virtual UbuntuViewManager* viewManager() { return nullptr; }

  virtual QString moduleName() = 0;
  virtual QStringList methodsToExport() = 0;
  virtual QVariantMap constantsToExport() = 0;
};

#define ReactModuleInterface_IID "com.canonical.ReactNative.ModuleInterface"

Q_DECLARE_INTERFACE(ReactModuleInterface, ReactModuleInterface_IID);

#endif // REACTMODULEINTERFACE_H
