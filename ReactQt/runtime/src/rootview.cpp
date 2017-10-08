
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

#include "layout/flexbox.h"
#include "reactattachedproperties.h"
#include "reactbridge.h"
#include "reactuimanager.h"
#include "rootview.h"
#include "utilities.h"

using namespace utilities;

namespace {
QVariantMap makeReactTouchEvent(QQuickItem* item, QMouseEvent* event) {
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
QVariantMap makeReactTouchEvent(QQuickItem* item, QTouchEvent* event) {
    return QVariantMap{};
}
}

class RootViewPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(RootView)
public:
    bool liveReload = false;
    QString moduleName;
    QUrl codeLocation;
    QVariantMap properties;
    QString pluginsPath;
    QString executor = "ReactNetExecutor";
    ReactBridge* bridge = nullptr;
    RootView* q_ptr;

    RootViewPrivate(RootView* q) : q_ptr(q) {}

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
    void onLiveReloadChanged() {
        if (!liveReload)
            return;
        if (bridge != nullptr && bridge->ready())
            monitorChangeUrl();
    }
};

RootView::RootView(QQuickItem* parent) : ReactItem(parent), d_ptr(new RootViewPrivate(this)) {
    Q_D(RootView);

    setAcceptedMouseButtons(Qt::LeftButton);
    setFiltersChildMouseEvents(true);

    d->bridge = new ReactBridge(this);
    connect(d->bridge, SIGNAL(readyChanged()), SLOT(bridgeReady()));

    connect(this, SIGNAL(liveReloadChanged()), d, SLOT(onLiveReloadChanged()));

    connect(this, SIGNAL(widthChanged()), this, SLOT(requestPolish()));
    connect(this, SIGNAL(heightChanged()), this, SLOT(requestPolish()));
    connect(this, SIGNAL(scaleChanged()), this, SLOT(requestPolish()));
}

RootView::~RootView() {}

bool RootView::liveReload() const {
    return d_func()->liveReload;
}

void RootView::setLiveReload(bool liveReload) {
    Q_D(RootView);
    if (d->liveReload == liveReload)
        return;

    d->liveReload = liveReload;
    emit liveReloadChanged();
}

QString RootView::moduleName() const {
    return d_func()->moduleName;
}

void RootView::setModuleName(const QString& moduleName) {
    Q_D(RootView);
    if (d->moduleName != moduleName) {
        d->moduleName = moduleName;
        emit moduleNameChanged();
    }
}

QUrl RootView::codeLocation() const {
    return d_func()->codeLocation;
}

void RootView::setCodeLocation(const QUrl& codeLocation) {
    Q_D(RootView);
    if (d->codeLocation != codeLocation) {
        d->codeLocation = codeLocation;
        emit codeLocationChanged();
    }
}

QVariantMap RootView::properties() const {
    return d_func()->properties;
}

void RootView::setProperties(const QVariantMap& properties) {
    Q_D(RootView);
    d->properties = properties;
    emit propertiesChanged();
}

QString RootView::pluginsPath() const {
    return d_func()->pluginsPath;
}

void RootView::setPluginsPath(const QString& pluginsPath) {
    Q_D(RootView);
    if (d->pluginsPath == pluginsPath)
        return;
    d->pluginsPath = pluginsPath;
    Q_EMIT pluginsPathChanged();
}

QString RootView::executor() const {
    return d_func()->executor;
}

void RootView::setExecutor(const QString& executor) {
    Q_D(RootView);
    if (d->executor == executor)
        return;
    d->executor = executor;
    Q_EMIT executorChanged();
}

ReactBridge* RootView::bridge() const {
    return d_func()->bridge;
}

void RootView::loadBundle(const QString& moduleName, const QUrl& codeLocation) {
    Q_ASSERT(!moduleName.isEmpty() && !codeLocation.isEmpty());

    setModuleName(moduleName);
    setCodeLocation(codeLocation);

    if (bridge()) {
        bridge()->loadBundle(codeLocation);
    }
}

void RootView::updatePolish() {
    if (childItems().count() > 0) {
        Q_ASSERT(childItems().count() == 1);
        auto view = childItems().at(0);
        Flexbox* flexbox = Flexbox::findFlexbox(view);
        if (flexbox) {
            flexbox->recalculateLayout(width(), height());
            // flexbox->printFlexboxHierarchy();
        }
    }
}

void RootView::bridgeReady() {
    Q_D(RootView);

    if (!d->bridge->ready())
        return;

    // XXX: should do the root view tag allocation internally
    ReactAttachedProperties* ap = ReactAttachedProperties::get(this);
    ap->setTag(d->bridge->uiManager()->allocateRootTag());
    d->bridge->uiManager()->registerRootView(this);

    QVariantList args{d->moduleName, QVariantMap{{"rootTag", ap->tag()}, {"initialProps", d->properties}}};

    d->bridge->enqueueRunAppCall(args);

    if (d->liveReload) {
        d->monitorChangeUrl();
    }
}

void RootView::requestPolish() {
    if (!d_ptr->bridge->ready())
        return;
    polish();
}

void RootView::componentComplete() {
    Q_D(RootView);

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

void RootView::mousePressEvent(QMouseEvent* event) {
    // qDebug() << __PRETTY_FUNCTION__;
    Q_D(RootView);

    QVariantMap e = makeReactTouchEvent(this, event);
    if (e.isEmpty())
        return;

    d->bridge->enqueueJSCall("RCTEventEmitter",
                             "receiveTouches",
                             QVariantList{normalizeInputEventName("touchStart"), QVariantList{e}, QVariantList{0}});
    event->setAccepted(true);
}

// TODO: optimize this
void RootView::mouseMoveEvent(QMouseEvent* event) {
    Q_D(RootView);

    QVariantMap e = makeReactTouchEvent(this, event);
    if (e.isEmpty())
        return;

    d->bridge->enqueueJSCall("RCTEventEmitter",
                             "receiveTouches",
                             QVariantList{normalizeInputEventName("touchMove"), QVariantList{e}, QVariantList{0}});
}

void RootView::mouseReleaseEvent(QMouseEvent* event) {
    // qDebug() << __PRETTY_FUNCTION__;
    Q_D(RootView);

    QVariantMap e = makeReactTouchEvent(this, event);
    if (e.isEmpty())
        return;

    d->bridge->enqueueJSCall("RCTEventEmitter",
                             "receiveTouches",
                             QVariantList{normalizeInputEventName("touchEnd"), QVariantList{e}, QVariantList{0}});
    event->setAccepted(true);
}

bool RootView::childMouseEventFilter(QQuickItem* item, QEvent* event) {
    // qDebug() << __PRETTY_FUNCTION__ << item << event;
    Q_D(RootView);

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
        d->bridge->enqueueJSCall("RCTEventEmitter",
                                 "receiveTouches",
                                 QVariantList{normalizeInputEventName(eventName), QVariantList{e}, QVariantList{0}});
    });

    event->setAccepted(true);
    return false;
}

#include "rootview.moc"
