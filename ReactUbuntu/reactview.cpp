
#include <QQmlEngine>
#include <QTimer>


#include "reactevents.h"
#include "reactbridge.h"
#include "reactuimanager.h"
#include "reactattachedproperties.h"
#include "reactflexlayout.h"
#include "reactview.h"


namespace
{
QVariantMap makeReactTouchEvent(QQuickItem* item, QMouseEvent* event)
{
  const QPointF& lp = event->localPos();

  QVariantMap e;

  // Find the deepest match
  QQuickItem* target = nullptr;
  QQuickItem* next = item;
  QPointF local = lp;
  forever {
    target = next;
    next = target->childAt(local.x(), local.y());
    if (next == nullptr || !next->isEnabled())
      break;
    local = target->mapToItem(next, local);
  }

  // XXX: should climb back up to a matching react target?
  ReactAttachedProperties* properties = ReactAttachedProperties::get(target, false);
  if (properties == nullptr) {
    qDebug() << __PRETTY_FUNCTION__ << "target was not a reactItem";
    return e;
  }

  e.insert("target", properties->tag());
  e.insert("identifier", 1);
  e.insert("touches", QVariant());
  e.insert("changedTouches", QVariant());

  e.insert("pageX", lp.x());
  e.insert("pageY", lp.y());
  e.insert("locationX", local.x());
  e.insert("locationY", local.y());
  e.insert("timestamp", QVariant::fromValue(event->timestamp()));

  return e;
}
// TODO:
QVariantMap makeReactTouchEvent(QQuickItem* item, QTouchEvent* event)
{
  return QVariantMap{};
}
}


ReactView::ReactView(QQuickItem* parent)
  : ReactItem(parent)
  , m_bridge(new ReactBridge(this))
{
  setAcceptedMouseButtons(Qt::LeftButton);
  setFiltersChildMouseEvents(true);
  connect(m_bridge, SIGNAL(bridgeReady()), SLOT(bridgeReady()));
}

ReactView::~ReactView()
{
}

QString ReactView::moduleName() const
{
  return m_moduleName;
}

void ReactView::setModuleName(const QString& moduleName)
{
  if (m_moduleName != moduleName) {
    m_moduleName = moduleName;
    emit moduleNameChanged();
  }
}

QUrl ReactView::codeLocation() const
{
  return m_codeLocation;
}

void ReactView::setCodeLocation(const QUrl& codeLocation)
{
  if (m_codeLocation != codeLocation) {
    m_codeLocation = codeLocation;
    emit codeLocationChanged();
  }
}

QVariantMap ReactView::properties() const
{
  return m_properties;
}

void ReactView::setProperties(const QVariantMap& properties)
{
  m_properties = properties;
}

void ReactView::bridgeReady()
{
  // XXX: should do the root view tag allocation internally
  ReactAttachedProperties* properties = ReactAttachedProperties::get(this);
  properties->setTag(m_bridge->uiManager()->allocateRootTag());
  m_bridge->uiManager()->registerRootView(this);

  QVariantList args{
    m_moduleName,
    QVariantMap{
      { "rootTag", properties->tag() },
      { "initialProps", m_properties }
    }
  };

  m_bridge->enqueueJSCall("AppRegistry", "runApplication", args);
}

void ReactView::componentComplete()
{
  QQuickItem::componentComplete();

  QTimer::singleShot(0, [=]() {
      // TODO: setQmlEngine && setNetworkAccessManager to be just setQmlEngine && then internal?
      m_bridge->setQmlEngine(qmlEngine(this));
      m_bridge->setNetworkAccessManager(qmlEngine(this)->networkAccessManager());
      m_bridge->setBundleUrl(m_codeLocation);
      m_bridge->setVisualParent(this);
      m_bridge->init();
    });
}

void ReactView::mousePressEvent(QMouseEvent* event)
{
  QVariantMap e = makeReactTouchEvent(this, event);
  if (e.isEmpty())
    return;

  m_bridge->enqueueJSCall("RCTEventEmitter", "receiveTouches",
                          QVariantList{ normalizeInputEventName("touchStart"),
                                        QVariantList{e},
                                        QVariantList{0} });
  event->setAccepted(true);
}

// TODO: optimize this
void ReactView::mouseMoveEvent(QMouseEvent* event)
{
  QVariantMap e = makeReactTouchEvent(this, event);
  if (e.isEmpty())
    return;

  m_bridge->enqueueJSCall("RCTEventEmitter", "receiveTouches",
                          QVariantList{ normalizeInputEventName("touchMove"),
                              QVariantList{e},
                              QVariantList{0} });
}

void ReactView::mouseReleaseEvent(QMouseEvent* event)
{
  QVariantMap e = makeReactTouchEvent(this, event);
  if (e.isEmpty())
    return;

  m_bridge->enqueueJSCall("RCTEventEmitter", "receiveTouches",
                          QVariantList{ normalizeInputEventName("touchEnd"),
                                        QVariantList{e},
                                        QVariantList{0} });
  event->setAccepted(true);
}

bool ReactView::childMouseEventFilter(QQuickItem* item, QEvent* event)
{
  qDebug() << __PRETTY_FUNCTION__;
  QVariantMap e = makeReactTouchEvent(item, static_cast<QMouseEvent*>(event));
  if (e.isEmpty())
    return false;

  QString eventName;
  if (event->type() == QEvent::MouseButtonPress) {
    eventName = "touchStart";
  } else if (event->type() == QEvent::MouseButtonRelease) {
    eventName = "touchEnd";
  } else if (event->type() == QEvent::MouseMove) {
    eventName = "touchMove";
  }

  QTimer::singleShot(0, [=]() {
      m_bridge->enqueueJSCall("RCTEventEmitter", "receiveTouches",
                              QVariantList{ normalizeInputEventName(eventName),
                                            QVariantList{e},
                                            QVariantList{0} });
    });

  event->setAccepted(true);
  return false;
}
