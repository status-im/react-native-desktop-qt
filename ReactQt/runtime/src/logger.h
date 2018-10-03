
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef LOGGER
#define LOGGER

#include <QDebug>
#include <QSettings>
#include <QString>

const QString UIMANAGER = "UIManager";
const QString FLEXBOX = "Flexbox";
const QString VIEWMANAGER = "ViewManager";
const QString NETWORKING = "Networking";
const QString WEBSOCKET = "WebSocketModule";

class Logger {

public:
    static Logger& getInstance();
    ~Logger();
    QString& devNull();
    const QSettings& settings();
    void registerModule(const QString& moduleName);
    void sync();

private:
    Logger();

private:
    QSettings* m_settings;
    QString m_devnull;
};

QDebug rnLog(const QString& module);
void registerLogModule(const QString& module);

#endif // LOGGER
