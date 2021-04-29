/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "reactnetworkaccessmanager.h"

ReactNetworkAccessManager::ReactNetworkAccessManager(QObject* parent) : QNetworkAccessManager(parent) {}

QNetworkReply* ReactNetworkAccessManager::createRequest(QNetworkAccessManager::Operation op,
                                                        const QNetworkRequest& originalReq,
                                                        QIODevice* outgoingData) {
    QNetworkRequest requestWithHeader = originalReq;
    requestWithHeader.setRawHeader("User-Agent", "react-native-desktop");
    return QNetworkAccessManager::createRequest(op, requestWithHeader, outgoingData);
}

QNetworkAccessManager* ReactNetworkAccessManagerFactory::create(QObject* parent) {
    return new ReactNetworkAccessManager(parent);
}

#include "reactnetworkaccessmanager.moc"
