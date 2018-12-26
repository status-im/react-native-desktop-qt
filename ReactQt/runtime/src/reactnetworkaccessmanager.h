/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef REACTNETWORKACCESSMANAGER_H
#define REACTNETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>
#include <QQmlNetworkAccessManagerFactory>

class ReactNetworkAccessManager : public QNetworkAccessManager {
    Q_OBJECT

public:
    ReactNetworkAccessManager(QObject* parent = nullptr);

protected:
    virtual QNetworkReply* createRequest(QNetworkAccessManager::Operation op,
                                         const QNetworkRequest& originalReq,
                                         QIODevice* outgoingData = nullptr);
};

class ReactNetworkAccessManagerFactory : public QQmlNetworkAccessManagerFactory {
public:
    QNetworkAccessManager* create(QObject* parent) override;
};

#endif // REACTNETWORKACCESSMANAGER_H
