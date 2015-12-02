#ifndef REACTPLUGIN_H
#define REACTPLUGIN_H
#include <QQmlExtensionPlugin>



class ReactPlugin : public QQmlExtensionPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
   void registerTypes(const char* uri);
};

#endif // REACTPLUGIN_H
