
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "linkingmanager.h"
#include "bridge.h"
#include <QDesktopServices>

class LinkingManagerPrivate {
public:
    Bridge* bridge = nullptr;
};

void LinkingManager::openURL(const QString& url,
                             const ModuleInterface::ListArgumentBlock& resolve,
                             const ModuleInterface::ListArgumentBlock& reject) {
    Q_D(LinkingManager);
    QDesktopServices::openUrl(QUrl(url));
    resolve(d->bridge, QVariantList{});
}

void LinkingManager::canOpenURL(const QString& url,
                                const ModuleInterface::ListArgumentBlock& resolve,
                                const ModuleInterface::ListArgumentBlock& reject) {
    Q_UNUSED(url);
    Q_D(LinkingManager);
    resolve(d->bridge, QVariantList{true});
}

void LinkingManager::getInitialURL(const ModuleInterface::ListArgumentBlock& resolve,
                                   const ModuleInterface::ListArgumentBlock& reject) {
    Q_D(LinkingManager);
    Q_UNUSED(reject);
    resolve(d->bridge, QVariantList{});
}

LinkingManager::LinkingManager(QObject* parent) : QObject(parent), d_ptr(new LinkingManagerPrivate) {}

LinkingManager::~LinkingManager() {}

void LinkingManager::setBridge(Bridge* bridge) {
    Q_D(LinkingManager);
    d->bridge = bridge;
}

QString LinkingManager::moduleName() {
    return "RCTLinkingManager";
}
