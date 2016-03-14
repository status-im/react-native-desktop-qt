#ifndef REACTEVENTDISPATCHER_H
#define REACTEVENTDISPATCHER_H

#include <QObject>


class ReactBridge;


class ReactEventDispatcherPrivate;
class ReactEventDispatcher : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(ReactEventDispatcher);

public:
  ReactEventDispatcher(ReactBridge* bridge);
  ~ReactEventDispatcher();

  void sendDeviceEvent(const QString& name, const QVariantList& args);

private:
  QScopedPointer<ReactEventDispatcherPrivate> d_ptr;
};

#endif // REACTEVENTDISPATCHER_H
