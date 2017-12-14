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

#include <memory>

#include "bridge.h"
#include "rnrandombytes.h"
#include <QCryptographicHash>
#include <QDateTime>
#include <QMap>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QQuickImageProvider>

class RNRandomBytesPrivate {

public:
    RNRandomBytesPrivate() {
        qsrand(QDateTime::currentDateTime().currentMSecsSinceEpoch());
        while (seed.size() < 4096) {
            seed.append(QChar(qrand()));
        }
    }
    QString seed;
};

RNRandomBytes::RNRandomBytes(QObject* parent) : QObject(parent), d_ptr(new RNRandomBytesPrivate) {}

RNRandomBytes::~RNRandomBytes() {}

QString RNRandomBytes::moduleName() {
    return "RCTRNRandomBytes";
}

QList<ModuleMethod*> RNRandomBytes::methodsToExport() {
    return QList<ModuleMethod*>{};
}

QVariantMap RNRandomBytes::constantsToExport() {
    return QVariantMap{{"seed", d_ptr->seed}};
}
