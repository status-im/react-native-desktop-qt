
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

#include "bridge.h"
#include "clipboard.h"
#include "eventdispatcher.h"
#include <QClipboard>
#include <QGuiApplication>

class ClipboardPrivate {
public:
    Bridge* bridge = nullptr;
};

void Clipboard::getString(const ModuleInterface::ListArgumentBlock& resolve,
                          const ModuleInterface::ListArgumentBlock& reject) {
    Q_UNUSED(reject);
    Q_D(Clipboard);
    QString clipboardContent = QGuiApplication::clipboard()->text();
    resolve(d->bridge, QVariantList{clipboardContent});
}

void Clipboard::setString(const QString& clipboardContent) {
    Q_D(Clipboard);
    QGuiApplication::clipboard()->setText(clipboardContent);
}

Clipboard::Clipboard(QObject* parent) : QObject(parent), d_ptr(new ClipboardPrivate) {}

Clipboard::~Clipboard() {}

void Clipboard::setBridge(Bridge* bridge) {
    Q_D(Clipboard);
    d->bridge = bridge;
}

QString Clipboard::moduleName() {
    return "RCTClipboard";
}
