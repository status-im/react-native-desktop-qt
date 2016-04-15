
#include <QGuiApplication>
#include <QCommandLineParser>
#include <QQuickView>

#include "reactattachedproperties.h"
#include "reactflexlayout.h"
#include "reacttextproperties.h"
#include "reactrawtextproperties.h"
#include "reactitem.h"
#include "reactview.h"


// TODO: some way to change while running
class ReactNativeProperties : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool liveReload READ liveReload WRITE setLiveReload NOTIFY liveReloadChanged)
  Q_PROPERTY(QString packagerHost READ packagerHost WRITE setPackagerHost NOTIFY packagerHostChanged)
  Q_PROPERTY(QString packagerPort READ packagerPort WRITE setPackagerPort NOTIFY packagerPortChanged)
public:
  ReactNativeProperties(QObject* parent = 0):QObject(parent) {}
  bool liveReload() const {
    return m_liveReload;
  }
  void setLiveReload(bool liveReload) {
    if (m_liveReload == liveReload)
      return;
    m_liveReload = liveReload;
    Q_EMIT liveReloadChanged();
  }
  QString packagerHost() const {
    return m_packagerHost;
  }
  void setPackagerHost(const QString& packagerHost) {
    if (m_packagerHost == packagerHost)
      return;
    m_packagerHost = packagerHost;
    Q_EMIT packagerHostChanged();
  }
  QString packagerPort() const {
    return m_packagerPort;
  }
  void setPackagerPort(const QString& packagerPort) {
    if (m_packagerPort == packagerPort)
      return;
    m_packagerPort = packagerPort;
    Q_EMIT packagerPortChanged();
  }
Q_SIGNALS:
  void liveReloadChanged();
  void packagerHostChanged();
  void packagerPortChanged();
private:
  bool m_liveReload = false;
  QString m_packagerHost = "localhost";
  QString m_packagerPort = "8081";
};

void registerTypes()
{
  qmlRegisterUncreatableType<ReactAttachedProperties>("React", 0, 1, "React", "React is not meant to be created directly");
  qmlRegisterUncreatableType<ReactFlexLayout>("React", 0, 1, "Flex", "Flex is not meant to be created directly");
  qmlRegisterUncreatableType<ReactTextProperties>("React", 0, 1, "Text", "Text is not meant to be created directly");
  qmlRegisterUncreatableType<ReactRawTextProperties>("React", 0, 1, "RawText", "Text is not meant to be created directly");
  qmlRegisterType<ReactItem>("React", 0, 1, "Item");
  qmlRegisterType<ReactView>("React", 0, 1, "RootView");
}

int main(int argc, char** argv)
{
  QGuiApplication app(argc, argv);
  QQuickView view;
  ReactNativeProperties* rnp = new ReactNativeProperties(&view);

  registerTypes();

  QCommandLineParser p;
  p.setApplicationDescription("React Native host application");
  p.addHelpOption();
  p.addOptions({
    {{"R", "live-reload"}, "Enable live reload."},
    {{"H", "host"}, "Set packager host address.", rnp->packagerHost()},
    {{"P", "port"}, "Set packager port number.", rnp->packagerPort()},
  });
  p.process(app);
  rnp->setLiveReload(p.isSet("live-reload"));
  if (p.isSet("host"))
    rnp->setPackagerHost(p.value("host"));
  if (p.isSet("port"))
    rnp->setPackagerPort(p.value("port"));

  view.rootContext()->setContextProperty("ReactNativeProperties", rnp);
  view.setSource(QUrl("qrc:///main.qml"));
  view.setResizeMode(QQuickView::SizeRootObjectToView);
  view.show();

  return app.exec();
}

#include "main.moc"

