
#include <QtQml>

#include "reactplugin.h"
#include "reactview.h"


void ReactPlugin::registerTypes(const char* uri)
{
  qmlRegisterType<ReactView>("React", 1, 0, "ReactView");
}
