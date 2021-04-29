
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

#include "asynclocalstorage.h"
#include <QDebug>
#include <QGuiApplication>
#include <QSettings>
#include <QStandardPaths>
#include <algorithm>
#include <iterator>

namespace {
// TODO: promote to global
QVariantMap makeError(const QString& message, const QVariantMap& extraData = QVariantMap{}) {
    QVariantMap rc(extraData);
    rc.insert("message", message);
    return rc;
}
} // namespace

void AsyncLocalStorage::multiGet(const QList<QString>& keys, const ModuleInterface::ListArgumentBlock& callback) {
    if (m_settings == nullptr) {
        callback(m_bridge, QVariantList{QVariantList{makeError("Could not open storage file")}, QVariant()});
        return;
    }

    QVariantList rc;
    for (const QString& key : keys) {
        rc.push_back(QVariantList{key, m_settings->value(key)});
    }
    callback(m_bridge, QVariantList{QVariant(), rc});
}

void AsyncLocalStorage::multiSet(const QList<QList<QString>>& kvPairs,
                                 const ModuleInterface::ListArgumentBlock& callback) {
    if (m_settings == nullptr) {
        callback(m_bridge, QVariantList{QVariantList{makeError("Could not open storage file")}, QVariant()});
        return;
    }

    for (const QList<QString>& pair : kvPairs) {
        m_settings->setValue(pair.first(), pair.last());
    }
    callback(m_bridge, QVariantList{QVariant()});
}

void AsyncLocalStorage::multiRemove(const QList<QString>& keys, const ModuleInterface::ListArgumentBlock& callback) {
    if (m_settings == nullptr) {
        callback(m_bridge, QVariantList{QVariantList{makeError("Could not open storage file")}, QVariant()});
        return;
    }

    for (const QString& key : keys) {
        m_settings->remove(key);
    }
    callback(m_bridge, QVariantList{QVariant()});
}

void AsyncLocalStorage::clear(const ModuleInterface::ListArgumentBlock& callback) {
    if (m_settings == nullptr) {
        callback(m_bridge, QVariantList{QVariantList{makeError("Could not open storage file")}, QVariant()});
        return;
    }

    m_settings->clear();
    callback(m_bridge, QVariantList{QVariant()});
}

void AsyncLocalStorage::getAllKeys(const ModuleInterface::ListArgumentBlock& callback) {
    if (m_settings == nullptr) {
        callback(m_bridge, QVariantList{QVariantList{makeError("Could not open storage file")}, QVariant()});
        return;
    }

    QStringList sk = m_settings->allKeys();
    QVariantList rc;
    std::transform(
        sk.begin(), sk.end(), std::back_inserter(rc), [](const QString& v) { return QVariant::fromValue(v); });
    callback(m_bridge, QVariantList{QVariant(), rc});
}

AsyncLocalStorage::AsyncLocalStorage(QObject* parent) : QObject(parent), m_bridge(nullptr), m_settings(nullptr) {}

AsyncLocalStorage::~AsyncLocalStorage() {}

void AsyncLocalStorage::setBridge(Bridge* bridge) {
    m_bridge = bridge;

    // Setup like state saver
    QString applicationName(qApp->applicationName());
    if (applicationName.isEmpty()) {
        qCritical() << __PRETTY_FUNCTION__ << "Cannot create local storage file, application name not defined.";
        return;
    }
    QString runtimeDir = QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation);
    if (runtimeDir.isEmpty()) {
        runtimeDir = qgetenv("XDG_RUNTIME_DIR");
    }
    if (runtimeDir.isEmpty()) {
        qCritical() << __PRETTY_FUNCTION__ << "No XDG_RUNTIME_DIR path set, cannot create local storage file.";
        return;
    }
    m_settings = new QSettings(
        QString("%1/%2/react-state-store").arg(runtimeDir).arg(applicationName), QSettings::NativeFormat, this);
    m_settings->setFallbacksEnabled(false);
}

ViewManager* AsyncLocalStorage::viewManager() {
    return nullptr;
}

QString AsyncLocalStorage::moduleName() {
    return "RCTAsyncLocalStorage";
}
