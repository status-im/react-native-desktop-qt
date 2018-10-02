
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "logger.h"
#include <QBuffer>

const QString LOG_SETTINGS_FILE = "logger_settings.ini";

Logger& Logger::getInstance() {
    static Logger logger;
    return logger;
}

Logger::~Logger() {
    delete m_settings;
    m_settings = nullptr;
}

QString& Logger::devNull() {
    return m_devnull;
}

const QSettings& Logger::settings() {
    return *m_settings;
}

void Logger::registerModule(const QString& moduleName) {
    if (!m_settings->contains(moduleName)) {
        m_settings->setValue(moduleName, true);
    }
}

void Logger::sync() {
    m_settings->sync();
}

Logger::Logger() {
    m_settings = new QSettings(LOG_SETTINGS_FILE, QSettings::IniFormat);

    registerModule(UIMANAGER);
    registerModule(FLEXBOX);
    registerModule(VIEWMANAGER);
    registerModule(NETWORKING);
    registerModule(WEBSOCKET);
}

QDebug rnLog(const QString& module) {
    Logger& logger = Logger::getInstance();
    logger.devNull().clear();

    if (!logger.settings().contains(module)) {
        qDebug() << "Logging not shown for unknown module: " << module;
        return QDebug(&Logger::getInstance().devNull());
    }

    // if module not enabled, we redirect logging to string that will be cleared
    // later. If enabled - text will appear in debug oputput
    if (logger.settings().value(module).toBool()) {
        QDebug debugStream = qDebug();
        return (debugStream << module + ":");
    } else {
        return QDebug(&Logger::getInstance().devNull());
    }
}

void registerLogModule(const QString& module) {
    Logger::getInstance().registerModule(module);
}
