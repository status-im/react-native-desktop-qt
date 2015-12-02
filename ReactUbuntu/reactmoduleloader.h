#ifndef REACTMODULELOADER_H
#define REACTMODULELOADER_H

#include <QObject>

class ReactModuleLoader
{
public:
  virtual ~ReactModuleLoader() {}

  virtual QObjectList availableModules() = 0;
};

#define ReactModuleLoader_IID "com.canonical.ReactNative.ModuleLoader"

Q_DECLARE_INTERFACE(ReactModuleLoader, ReactModuleLoader_IID);

#endif // REACTMODULELOADER_H
