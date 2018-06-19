/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "blobprovider.h"
#include "bridge.h"

class BlobProviderPrivate {
public:
    Bridge* bridge = nullptr;
};

BlobProvider::BlobProvider(QObject* parent) : QObject(parent), d_ptr(new BlobProviderPrivate) {}

BlobProvider::~BlobProvider() {}

void BlobProvider::setBridge(Bridge* bridge) {
    Q_D(BlobProvider);
    d->bridge = bridge;
}

QString BlobProvider::moduleName() {
    return "RCTBlobModule";
}

void BlobProvider::enableBlobSupport(qlonglong socketId) {}

void BlobProvider::disableBlobSupport(qlonglong socketId) {}

void BlobProvider::addNetworkingHandler() {}
