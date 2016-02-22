#ifndef UBUNTUCOMPONENTSLOADER_H
#define UBUNTUCOMPONENTSLOADER_H

#include "reactmoduleloader.h"


class UbuntuComponentsLoader
  : public QObject
  , public ReactModuleLoader
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleLoader_IID)
  Q_INTERFACES(ReactModuleLoader)

public:

  QObjectList availableModules() override;
};

#endif // UBUNTUCOMPONENTSLOADER_H
