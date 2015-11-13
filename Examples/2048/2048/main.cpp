
#include <QGuiApplication>
#include <QtQuick/QQuickView>

#include "reactview.h"


int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

  qmlRegisterType<ReactView>("React", 1, 0, "ReactView");

  QQuickView view;
  view.setResizeMode(QQuickView::SizeRootObjectToView);
  view.setSource(QUrl("qrc:///react-demo.qml"));
  view.show();
  return app.exec();
}
