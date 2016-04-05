
#include <QtQml>
#include <QMetaType>

#include "reactplugin.h"
#include "reactattachedproperties.h"
#include "reactflexlayout.h"
#include "reacttextproperties.h"
#include "reactrawtextproperties.h"
#include "reactitem.h"
#include "reactview.h"


void ReactPlugin::registerTypes(const char* uri)
{
  qmlRegisterUncreatableType<ReactAttachedProperties>("React", 0, 1, "React", "React is not meant to be created directly");
  qmlRegisterUncreatableType<ReactFlexLayout>("React", 0, 1, "Flex", "Flex is not meant to be created directly");
  qmlRegisterUncreatableType<ReactTextProperties>("React", 0, 1, "Text", "Text is not meant to be created directly");
  qmlRegisterUncreatableType<ReactRawTextProperties>("React", 0, 1, "RawText", "Text is not meant to be created directly");
  qmlRegisterType<ReactItem>("React", 0, 1, "Item");
  qmlRegisterType<ReactView>("React", 0, 1, "RootView");
}
