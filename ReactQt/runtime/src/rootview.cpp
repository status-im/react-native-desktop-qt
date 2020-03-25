
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

#include <QGuiApplication>
#include <QQmlEngine>
#include <QScreen>
#include <QTimer>
#include <QVariant>

#include "attachedproperties.h"
#include "bridge.h"
#include "eventdispatcher.h"
#include "layout/flexbox.h"
#include "reactnetworkaccessmanager.h"
#include "rootview.h"
#include "uimanager.h"
#include "utilities.h"

using namespace utilities;

namespace {

const int RESET_CONTENT_HTTP_STATUS_CODE = 205;

// TODO:
QVariantMap makeReactTouchEvent(QQuickItem* item, QTouchEvent* event) {
    return QVariantMap{};
}
} // namespace

class RootViewPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(RootView)
public:
    bool liveReload = false;
    bool hotReload = false;
    QString moduleName;
    QUrl codeLocation;
    QVariantMap properties;
    QString pluginsPath;
    QString jsExecutor = "NodeJsExecutor";
    QString serverConnectionType = "RemoteServerConnection";
    Bridge* bridge = nullptr;
    RootView* q_ptr;
    bool remoteJSDebugging = false;
    QNetworkReply* liveReloadUrlReply = nullptr;

    RootViewPrivate(RootView* q) : q_ptr(q) {}

    void monitorChangeUrl() {
        if (codeLocation.scheme() != "http") {
            qWarning() << "Can only live reload when fetching from packager";
            return;
        }

        if (!liveReloadUrlReply) {
            QNetworkRequest request(codeLocation.resolved(QUrl("/onchange")));
            liveReloadUrlReply = qmlEngine(q_ptr)->networkAccessManager()->get(request);
            QObject::connect(liveReloadUrlReply, &QNetworkReply::finished, [=] {
                QNetworkReply::NetworkError replyError = liveReloadUrlReply->error();
                int replyStatusCode = liveReloadUrlReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                liveReloadUrlReply->deleteLater();
                liveReloadUrlReply = nullptr;
                if (replyError != QNetworkReply::NoError) {
                    qCritical() << __PRETTY_FUNCTION__ << "Error monitoring change url";
                    return;
                }

                if (!liveReload)
                    return;

                if (replyStatusCode == RESET_CONTENT_HTTP_STATUS_CODE) {
                    bridge->reload();
                }

                if (bridge->ready()) {
                    monitorChangeUrl();
                }
            });
        }
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

    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setFiltersChildMouseEvents(true);

    d->bridge = new Bridge(this);
    connect(d->bridge, SIGNAL(readyChanged()), SLOT(bridgeReady()));

    connect(this, SIGNAL(liveReloadChanged()), d, SLOT(onLiveReloadChanged()));

    connect(this, SIGNAL(widthChanged()), this, SLOT(onSizeChanged()));
    connect(this, SIGNAL(heightChanged()), this, SLOT(onSizeChanged()));
    connect(this, SIGNAL(scaleChanged()), this, SLOT(onSizeChanged()));
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

bool RootView::hotReload() const {
    return d_func()->hotReload;
}

void RootView::setHotReload(bool hotReload) {
    Q_D(RootView);
    if (d->hotReload == hotReload)
        return;

    d->hotReload = hotReload;
    emit hotReloadChanged();

    d->bridge->setHotReload(hotReload);
    d->bridge->reload();
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

QString RootView::jsExecutor() const {
    return d_func()->jsExecutor;
}

void RootView::setJsExecutor(const QString& jsExecutor) {
    Q_D(RootView);
    if (d->jsExecutor == jsExecutor)
        return;
    d->jsExecutor = jsExecutor;
    Q_EMIT jsExecutorChanged();
}

QString RootView::serverConnectionType() const {
    return d_func()->serverConnectionType;
}

void RootView::setServerConnectionType(const QString& executor) {
    Q_D(RootView);
    if (d->serverConnectionType == executor)
        return;
    d->serverConnectionType = executor;
    Q_EMIT executorChanged();
}

QVariantList RootView::externalModules() const {
    return d_func()->bridge->externalModules();
}

void RootView::setExternalModules(const QVariantList& externalModules) {
    Q_D(RootView);
    if (bridge()->externalModules() == externalModules)
        return;
    bridge()->setExternalModules(externalModules);
    Q_EMIT externalModulesChanged();
}

Bridge* RootView::bridge() const {
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

void RootView::startRemoteJSDebugging() {
    Q_D(RootView);

    if (d_func()->remoteJSDebugging == false) {
        d_func()->remoteJSDebugging = true;
        d->bridge->setRemoteJSDebugging(d_func()->remoteJSDebugging);
        d->bridge->reload();
    }
}

void RootView::reloadBridge() {
    Q_D(RootView);
    d->bridge->reload();
}

void RootView::updatePolish() {
    if (childItems().count() == 1) {
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
    AttachedProperties* ap = AttachedProperties::get(this);
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

void RootView::onSizeChanged() {
    sendSizeUpdate();
    requestPolish();
}

void RootView::sendSizeUpdate() {
    Q_D(RootView);
    if (!d->bridge->ready())
        return;

    QScreen* screen = QGuiApplication::primaryScreen();

    QVariantMap values{{"window",
                        QVariantMap{{"fontScale", screen->devicePixelRatio()},
                                    {"width", width()},
                                    {"height", height()},
                                    {"scale", screen->devicePixelRatio()}}}};

    d->bridge->eventDispatcher()->sendDeviceEvent("didUpdateDimensions", values);
}

void RootView::componentComplete() {
    Q_D(RootView);

    QQuickItem::componentComplete();

#ifdef RCT_DEV
    loadDevMenu();
#endif // RCT_DEV

    ReactNetworkAccessManagerFactory* networkManagerFactory = new ReactNetworkAccessManagerFactory();
    qmlEngine(this)->setNetworkAccessManagerFactory(networkManagerFactory);

    QTimer::singleShot(0, [=]() {
        // TODO: setQmlEngine && setNetworkAccessManager to be just setQmlEngine && then internal?
        d->bridge->setQmlEngine(qmlEngine(this));
        d->bridge->setNetworkAccessManager(qmlEngine(this)->networkAccessManager());
        d->bridge->setBundleUrl(d->codeLocation);
        d->bridge->setPluginsPath(d->pluginsPath);
        d->bridge->setJsExecutor(d->jsExecutor);
        d->bridge->setServerConnectionType(d->serverConnectionType);
        d->bridge->setVisualParent(this);
        d->bridge->init();
    });
}

bool RootView::sendMouseEventToJs(QMouseEvent* event, const QString& eventType, QQuickItem* receiver) {
    Q_D(RootView);

    QVariantMap e = makeReactTouchEvent(receiver, event);
    if (e.isEmpty()) {
        return false;
    }

    d->bridge->enqueueJSCall("RCTEventEmitter",
                             "receiveTouches",
                             QVariantList{normalizeInputEventName(eventType), QVariantList{e}, QVariantList{0}});

    event->setAccepted(true);
    return true;
}

#ifdef RCT_DEV
void RootView::loadDevMenu() {
    QQuickItem* item = createQMLItemFromSourceFile(qmlEngine(this), QUrl("qrc:/qml/DevMenu.qml"));
    if (!item) {
        qCritical() << QString("Can't create QML item for DevMenu.qml");
    }
    // DevMenu is supposed to be direct child of RootView's parent QML item
    item->setParentItem(this->parentItem());
    item->setProperty("rootView", QVariant::fromValue((QObject*)this));
}
#endif // RCT_DEV

void RootView::mousePressEvent(QMouseEvent* event) {
    sendMouseEventToJs(event, TOUCH_START, this);
}

// TODO: optimize this
void RootView::mouseMoveEvent(QMouseEvent* event) {
    sendMouseEventToJs(event, TOUCH_MOVE, this);
}

void RootView::mouseReleaseEvent(QMouseEvent* event) {
    sendMouseEventToJs(event, TOUCH_END, this);
}

bool RootView::childMouseEventFilter(QQuickItem* item, QEvent* event) {
    // qDebug() << __PRETTY_FUNCTION__ << item << event;
    Q_D(RootView);

    switch (event->type()) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove:
        break;
    default:
        return false;
    }

    QString eventName;
    if (event->type() == QEvent::MouseButtonPress) {
        eventName = TOUCH_START;
    } else if (event->type() == QEvent::MouseButtonRelease) {
        eventName = TOUCH_END;
    } else if (event->type() == QEvent::MouseMove) {
        eventName = TOUCH_MOVE;
    }

    bool stopEventPropagation = sendMouseEventToJs(static_cast<QMouseEvent*>(event), eventName, item);
    return stopEventPropagation;
}

#include "rootview.moc"
