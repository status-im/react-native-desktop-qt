
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <QNetworkAccessManager>

#include "alert.h"
#include "bridge.h"
#include "eventdispatcher.h"
#include "utilities.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickWindow>

class AlertPrivate {
public:
    AlertPrivate(Alert* q) : q_ptr(q) {}
    void createAlertItem(const QVariantMap& config, double callback);
    QVariantList generateQmlButtonsList(const QVariantMap& buttonsConfig);

public:
    Alert* q_ptr = nullptr;
    Bridge* bridge = nullptr;
    QQuickItem* alert = nullptr;
};

void AlertPrivate::createAlertItem(const QVariantMap& config, double callback) {
    if (alert) {
        alert->deleteLater();
        alert = nullptr;
    }

    alert = utilities::createQMLItemFromSourceFile(bridge->qmlEngine(), QUrl("qrc:/qml/ReactAlert.qml"));
    if (alert == nullptr) {
        qCritical() << __PRETTY_FUNCTION__ << "Unable to create Alert item";
        return;
    }

    alert->setParentItem(bridge->topmostVisualParent());
    alert->setObjectName("Alert");
    alert->setProperty("alertManager", QVariant::fromValue((QObject*)q_ptr));
    alert->setProperty("buttons", generateQmlButtonsList(config));
    alert->setProperty("callback", callback);
    if (config.contains("cancelable")) {
        alert->setProperty("cancelable", config["cancelable"]);
    }
    if (config.contains("title")) {
        if (!config["title"].toString().isEmpty())
            alert->setProperty("title", config["title"]);
        else
            alert->setProperty("title", " ");
    }
    if (config.contains("message")) {
        alert->setProperty("message", config["message"]);
    }
    alert->setProperty("callback", callback);
}

QVariantList AlertPrivate::generateQmlButtonsList(const QVariantMap& buttonsConfig) {
    QVariantList buttons;
    if (buttonsConfig.contains("buttonPositive")) {
        buttons.push_back(
            QVariantMap{{"name", "buttonPositive"}, {"role", "accept"}, {"text", buttonsConfig["buttonPositive"]}});
    }
    if (buttonsConfig.contains("buttonNegative")) {
        buttons.push_back(
            QVariantMap{{"name", "buttonNegative"}, {"role", "reject"}, {"text", buttonsConfig["buttonNegative"]}});
    }
    if (buttonsConfig.contains("buttonNeutral")) {
        buttons.push_back(
            QVariantMap{{"name", "buttonNeutral"}, {"role", "neutral"}, {"text", buttonsConfig["buttonNeutral"]}});
    }

    return buttons;
}

void Alert::alert(const QVariantMap& config, double callback) {
    Q_D(Alert);

    d_ptr->createAlertItem(config, callback);
    if (d->alert) {
        d->alert->metaObject()->invokeMethod(d->alert, "open");
    }
}

void Alert::sendButtonClickToJs(double callback, const QString& buttonName) {
    Q_D(Alert);

    d->bridge->invokePromiseCallback(callback, QVariantList{"Clicked", buttonName});
}

void Alert::sendDismissedToJs(double callback) {
    Q_D(Alert);

    d->bridge->invokePromiseCallback(callback, QVariantList{"Dismissed", ""});
}

Alert::Alert(QObject* parent) : QObject(parent), d_ptr(new AlertPrivate(this)) {}

Alert::~Alert() {}

void Alert::setBridge(Bridge* bridge) {
    Q_D(Alert);
    d->bridge = bridge;
}

QString Alert::moduleName() {
    return "RCTAlertManager";
}
