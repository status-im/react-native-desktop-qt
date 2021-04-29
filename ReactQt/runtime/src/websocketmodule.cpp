
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <QWebSocket>

#include <QDebug>

#include "bridge.h"
#include "eventdispatcher.h"
#include "websocketmodule.h"

Q_LOGGING_CATEGORY(WEBSOCKET, "WebSocketModule")

class WebSocketModulePrivate {
public:
    Bridge* bridge;
    QMap<qlonglong, QWebSocket*> sockets;

    void createSocketConnection(const QUrl& url, qlonglong socketId) {
        QWebSocket* socket = new QWebSocket();
        sockets.insert(socketId, socket);

        QObject::connect(socket, &QWebSocket::connected, [=]() {
#ifdef RCT_DEV
            qCDebug(WEBSOCKET) << "Socket connected. SocketId:" << socketId;
#endif // RCT_DEV
            if (bridge) {
                bridge->eventDispatcher()->sendDeviceEvent("websocketOpen", QVariantMap{{"id", socketId}});
            }
        });

        QObject::connect(socket, &QWebSocket::disconnected, [=]() {
            sockets.remove(socketId);
            socket->deleteLater();
#ifdef RCT_DEV
            qCDebug(WEBSOCKET) << "Socket disconnected. SocketId:" << socketId;
#endif // RCT_DEV
            if (bridge) {
                bridge->eventDispatcher()->sendDeviceEvent("websocketClosed",
                                                           QVariantMap{{"id", socketId},
                                                                       {"code", socket->closeCode()},
                                                                       {"reason", socket->closeReason()},
                                                                       {"clean", ""}});
            }
        });

        QObject::connect(socket, &QWebSocket::textMessageReceived, [=](const QString& message) {
#ifdef RCT_DEV
            qCDebug(WEBSOCKET) << QString("Text message %1 received for SocketId %2").arg(message).arg(socketId);
#endif // RCT_DEV
            if (bridge) {
                bridge->eventDispatcher()->sendDeviceEvent(
                    "websocketMessage", QVariantMap{{"id", socketId}, {"type", "text"}, {"data", message}});
            }
        });

        QObject::connect(socket, &QWebSocket::binaryMessageReceived, [=](const QByteArray& message) {
#ifdef RCT_DEV
            qCDebug(WEBSOCKET)
                << QString("Binary message of size %1 received for SocketId %2").arg(message.size()).arg(socketId);
#endif // RCT_DEV
            if (bridge) {
                bridge->eventDispatcher()->sendDeviceEvent(
                    "websocketMessage", QVariantMap{{"id", socketId}, {"type", "binary"}, {"data", message}});
            }
        });

        QObject::connect(socket,
                         static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
                         [=](QAbstractSocket::SocketError) {
#ifdef RCT_DEV
                             qCDebug(WEBSOCKET) << "Socket error:" << socket->errorString();
#endif // RCT_DEV
                         });

        socket->open(url);
    }
};

void WebSocketModule::connect(const QUrl& url,
                              const QVariantList& protocols,
                              const QVariantMap& options,
                              qlonglong socketId) {
    Q_D(WebSocketModule);

#ifdef RCT_DEV
    qCDebug(WEBSOCKET) << "WebSocketModule::connect with args: url:" << url << " socketId:" << socketId;
#endif // RCT_DEV

    d->createSocketConnection(url, socketId);
}

void WebSocketModule::send(const QString& message, qlonglong socketId) {
#ifdef RCT_DEV
    qCDebug(WEBSOCKET) << "WebSocketModule::send with args: message:" << message << " socketId:" << socketId;
#endif // RCT_DEV
    Q_D(WebSocketModule);
    if (d->sockets.contains(socketId)) {
        d->sockets[socketId]->sendTextMessage(message);
        d->sockets[socketId]->flush();
    }
}

void WebSocketModule::sendBinary(const QString& base64String, qlonglong socketId) {
#ifdef RCT_DEV
    qCDebug(WEBSOCKET) << "WebSocketModule::sendBinary with args: base64String:" << base64String
                       << " socketId:" << socketId;
#endif // RCT_DEV
    Q_D(WebSocketModule);
    if (d->sockets.contains(socketId)) {
        d->sockets[socketId]->sendBinaryMessage(base64String.toLocal8Bit());
        d->sockets[socketId]->flush();
    }
}

void WebSocketModule::ping(qlonglong socketId) {
#ifdef RCT_DEV
    qCDebug(WEBSOCKET) << "WebSocketModule::ping with args: socketId:" << socketId;
#endif // RCT_DEV
    Q_D(WebSocketModule);
    if (d->sockets.contains(socketId)) {
        d->sockets[socketId]->ping();
        d->sockets[socketId]->flush();
    }
}

void WebSocketModule::close(qlonglong socketId) {
#ifdef RCT_DEV
    qCDebug(WEBSOCKET) << "WebSocketModule::close Closing socket socketId:" << socketId;
#endif // RCT_DEV
    Q_D(WebSocketModule);
    if (d->sockets.contains(socketId)) {
        d->sockets[socketId]->close();
    }
}

WebSocketModule::WebSocketModule(QObject* parent) : QObject(parent), d_ptr(new WebSocketModulePrivate) {
    Q_D(WebSocketModule);
}

WebSocketModule::~WebSocketModule() {
    Q_D(WebSocketModule);
    d->bridge = nullptr;
    foreach (QWebSocket* websocket, d->sockets.values()) {
        websocket->close();
        websocket->disconnect();
        websocket->deleteLater();
    }
}

void WebSocketModule::setBridge(Bridge* bridge) {
    Q_D(WebSocketModule);
    d->bridge = bridge;
}

ViewManager* WebSocketModule::viewManager() {
    return nullptr;
}

QString WebSocketModule::moduleName() {
    return "WebSocketModule";
}
