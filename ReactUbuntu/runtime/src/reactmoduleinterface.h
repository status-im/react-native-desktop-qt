#ifndef REACTMODULEINTERFACE_H
#define REACTMODULEINTERFACE_H

#include <functional>

#include <QtPlugin>


class ReactBridge;
class ReactViewManager;
class ReactPropertyHandler;
class ReactModuleMethod;


#ifndef Q_MOC_RUN
#define REACT_PROMISE
#endif

class ReactModuleInterface
{
public:
  typedef std::function<void (ReactBridge*, const QVariantMap&)> MapArgumentBlock;
  typedef std::function<void (ReactBridge*, const QVariantList&)> ListArgumentBlock;

  // XXX:
  typedef ListArgumentBlock ResponseBlock;
  typedef MapArgumentBlock ErrorBlock;
  typedef MapArgumentBlock BubblingEventBlock;
  typedef MapArgumentBlock DirectEventBlock;

  virtual ~ReactModuleInterface() {}

  virtual void setBridge(ReactBridge* bridge) { Q_UNUSED(bridge); }

  // TODO: this doesnt seem right
  virtual ReactViewManager* viewManager() { return nullptr; }
  virtual ReactPropertyHandler* propertyHandler(QObject* object) { return nullptr; }

  virtual QString moduleName() = 0;
  virtual QList<ReactModuleMethod*> methodsToExport() = 0;
  virtual QVariantMap constantsToExport() = 0;
};

#define ReactModuleInterface_IID "com.canonical.ReactNative.ModuleInterface"

Q_DECLARE_INTERFACE(ReactModuleInterface, ReactModuleInterface_IID);

Q_DECLARE_METATYPE(ReactModuleInterface::MapArgumentBlock);
Q_DECLARE_METATYPE(ReactModuleInterface::ListArgumentBlock);

#endif // REACTMODULEINTERFACE_H
