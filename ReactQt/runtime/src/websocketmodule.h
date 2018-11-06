
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef WEBSOCKETMODULE_H
#define WEBSOCKETMODULE_H

#include <functional>

#include <QLoggingCategory>
#include <QObject>
#include <QVariant>

#include "moduleinterface.h"

Q_DECLARE_LOGGING_CATEGORY(WEBSOCKET)

class WebSocketModulePrivate;
class WebSocketModule : public QObject, public ModuleInterface {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ModuleInterface)

    Q_INVOKABLE void
    connect(const QUrl& url, const QVariantList& protocols, const QVariantMap& options, qlonglong socketId);
    Q_INVOKABLE void send(const QString& message, qlonglong socketId);
    Q_INVOKABLE void sendBinary(const QString& base64String, qlonglong socketId);
    Q_INVOKABLE void ping(qlonglong socketId);
    Q_INVOKABLE void close(qlonglong socketId);

    Q_DECLARE_PRIVATE(WebSocketModule);

public:
    WebSocketModule(QObject* parent = 0);
    ~WebSocketModule();

    void setBridge(Bridge* bridge) override;

    ViewManager* viewManager() override;

    QString moduleName() override;

private:
    QScopedPointer<WebSocketModulePrivate> d_ptr;
};

#endif // WEBSOCKETMODULE_H
