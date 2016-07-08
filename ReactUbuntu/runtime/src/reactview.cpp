
/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * Author: Justin McPherson <justin.mcpherson@canonical.com>
 *
 */

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

  ReactAttachedProperties* ap = ReactAttachedProperties::get(target, false);
  if (ap == nullptr) {
    qWarning() << __PRETTY_FUNCTION__ << "target was not a reactItem";
    return e;
  }

  e.insert("target", ap->tag());
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

class ReactViewPrivate : public QObject
{
  Q_OBJECT
  Q_DECLARE_PUBLIC(ReactView)
public:
  bool liveReload = false;
  QString moduleName;
  QUrl codeLocation;
  QVariantMap properties;
  QString pluginsPath;
  QString executor = "ReactNetExecutor";
  ReactBridge* bridge = nullptr;
  ReactView* q_ptr;

  ReactViewPrivate(ReactView* q)
    : q_ptr(q)
    {}

  void monitorChangeUrl() {
    if (codeLocation.scheme() != "http") {
      qWarning() << "Can only live reload when fetching from packager";
      return;
    }

    QNetworkRequest request(codeLocation.resolved(QUrl("/onchange")));
    QNetworkReply* reply = qmlEngine(q_ptr)->networkAccessManager()->get(request);
    QObject::connect(reply, &QNetworkReply::finished, [=] {
      reply->deleteLater();
      if (reply->error() != QNetworkReply::NoError) {
        qCritical() << __PRETTY_FUNCTION__ << "Error monitoring change url";
        return;
      }
      if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 205) {
        bridge->reload();
      }
      if (liveReload && bridge->ready()) {
        monitorChangeUrl();
      }
    });
  }

private Q_SLOTS:
  void liveReloadChanged() {
    if (!liveReload)
      return;
    if (bridge != nullptr && bridge->ready())
      monitorChangeUrl();
  }
};


ReactView::ReactView(QQuickItem* parent)
  : ReactItem(parent)
  , d_ptr(new ReactViewPrivate(this))
{
  Q_D(ReactView);

  setAcceptedMouseButtons(Qt::LeftButton);
  setFiltersChildMouseEvents(true);

  d->bridge = new ReactBridge(this);
  connect(d->bridge, SIGNAL(readyChanged()), SLOT(bridgeReady()));

  connect(this, SIGNAL(liveReloadChanged()), d, SLOT(liveReloadChanged()));
}

ReactView::~ReactView()
{
}

bool ReactView::liveReload() const
{
  return d_func()->liveReload;
}

void ReactView::setLiveReload(bool liveReload)
{
  Q_D(ReactView);
  if (d->liveReload == liveReload)
    return;

  d->liveReload = liveReload;
  emit liveReloadChanged();
}

QString ReactView::moduleName() const
{
  return d_func()->moduleName;
}

void ReactView::setModuleName(const QString& moduleName)
{
  Q_D(ReactView);
  if (d->moduleName != moduleName) {
    d->moduleName = moduleName;
    emit moduleNameChanged();
  }
}

QUrl ReactView::codeLocation() const
{
  return d_func()->codeLocation;
}

void ReactView::setCodeLocation(const QUrl& codeLocation)
{
  Q_D(ReactView);
  if (d->codeLocation != codeLocation) {
    d->codeLocation = codeLocation;
    emit codeLocationChanged();
  }
}

QVariantMap ReactView::properties() const
{
  return d_func()->properties;
}

void ReactView::setProperties(const QVariantMap& properties)
{
  Q_D(ReactView);
  d->properties = properties;
  emit propertiesChanged();
}

QString ReactView::pluginsPath() const
{
  return d_func()->pluginsPath;
}

void ReactView::setPluginsPath(const QString& pluginsPath)
{
  Q_D(ReactView);
  if (d->pluginsPath == pluginsPath)
    return;
  d->pluginsPath = pluginsPath;
  Q_EMIT pluginsPathChanged();
}

QString ReactView::executor() const
{
  return d_func()->executor;
}

void ReactView::setExecutor(const QString& executor)
{
  Q_D(ReactView);
  if (d->executor == executor)
    return;
  d->executor = executor;
  Q_EMIT executorChanged();
}

void ReactView::bridgeReady()
{
  Q_D(ReactView);

  if (!d->bridge->ready())
    return;

  // XXX: should do the root view tag allocation internally
  ReactAttachedProperties* ap = ReactAttachedProperties::get(this);
  ap->setTag(d->bridge->uiManager()->allocateRootTag());
  d->bridge->uiManager()->registerRootView(this);

  QVariantList args{
    d->moduleName,
    QVariantMap{
      { "rootTag", ap->tag() },
      { "initialProps", d->properties }
    }
  };

  d->bridge->enqueueJSCall("AppRegistry", "runApplication", args);

  if (d->liveReload) {
    d->monitorChangeUrl();
  }
}

void ReactView::componentComplete()
{
  Q_D(ReactView);

  QQuickItem::componentComplete();

  QTimer::singleShot(0, [=]() {
      // TODO: setQmlEngine && setNetworkAccessManager to be just setQmlEngine && then internal?
      d->bridge->setQmlEngine(qmlEngine(this));
      d->bridge->setNetworkAccessManager(qmlEngine(this)->networkAccessManager());
      d->bridge->setBundleUrl(d->codeLocation);
      d->bridge->setPluginsPath(d->pluginsPath);
      d->bridge->setExecutorName(d->executor);
      d->bridge->setVisualParent(this);
      d->bridge->init();
    });
}

void ReactView::mousePressEvent(QMouseEvent* event)
{
  // qDebug() << __PRETTY_FUNCTION__;
  Q_D(ReactView);

  QVariantMap e = makeReactTouchEvent(this, event);
  if (e.isEmpty())
    return;

  d->bridge->enqueueJSCall("RCTEventEmitter", "receiveTouches",
                          QVariantList{ normalizeInputEventName("touchStart"),
                                        QVariantList{e},
                                        QVariantList{0} });
  event->setAccepted(true);
}

// TODO: optimize this
void ReactView::mouseMoveEvent(QMouseEvent* event)
{
  Q_D(ReactView);

  QVariantMap e = makeReactTouchEvent(this, event);
  if (e.isEmpty())
    return;

  d->bridge->enqueueJSCall("RCTEventEmitter", "receiveTouches",
                           QVariantList{normalizeInputEventName("touchMove"),
                                        QVariantList{e},
                                        QVariantList{0}});
}

void ReactView::mouseReleaseEvent(QMouseEvent* event)
{
  // qDebug() << __PRETTY_FUNCTION__;
  Q_D(ReactView);

  QVariantMap e = makeReactTouchEvent(this, event);
  if (e.isEmpty())
    return;

  d->bridge->enqueueJSCall("RCTEventEmitter", "receiveTouches",
                          QVariantList{ normalizeInputEventName("touchEnd"),
                                        QVariantList{e},
                                        QVariantList{0} });
  event->setAccepted(true);
}

bool ReactView::childMouseEventFilter(QQuickItem* item, QEvent* event)
{
  // qDebug() << __PRETTY_FUNCTION__ << item << event;
  Q_D(ReactView);

  switch (event->type()) {
  // case QEvent::MouseButtonDblClick:
  case QEvent::MouseButtonPress:
  case QEvent::MouseButtonRelease:
  case QEvent::MouseMove:
    break;
  default:
    return false;
  }

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
      d->bridge->enqueueJSCall("RCTEventEmitter", "receiveTouches",
                              QVariantList{ normalizeInputEventName(eventName),
                                            QVariantList{e},
                                            QVariantList{0} });
    });

  event->setAccepted(true);
  return false;
}

#include "reactview.moc"
