
#include <QVariant>


#include "reacteventdispatcher.h"
#include "reactbridge.h"


class ReactEventDispatcherPrivate {
public:
  ReactBridge* bridge;
};


ReactEventDispatcher::ReactEventDispatcher(ReactBridge* bridge)
  : QObject(bridge)
  , d_ptr(new ReactEventDispatcherPrivate)
{
  Q_D(ReactEventDispatcher);
  d->bridge = bridge;
}

ReactEventDispatcher::~ReactEventDispatcher()
{
}

void ReactEventDispatcher::sendDeviceEvent(const QString& name, const QVariantList& args)
{
  d_func()->bridge->enqueueJSCall("RCTDeviceEventEmitter",
                                  "emit",
                                  QVariantList{name, args});
}
