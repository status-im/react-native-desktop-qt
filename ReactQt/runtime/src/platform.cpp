/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "platform.h"
#include "bridge.h"
#include "eventdispatcher.h"

#include <QQuickItem>

class PlatformPrivate {
public:
    PlatformPrivate() {
#if defined(Q_OS_MACOS)
        osName = QLatin1String("macos");
#elif defined(Q_OS_WIN)
        osName = QLatin1String("windows");
#elif defined(Q_OS_LINUX)
        osName = QLatin1String("linux");
#elif defined(Q_OS_UNIX)
        osName = QLatin1String("unix");
#else
        osName = QLatin1String("unknown");
#endif
    }

public:
    Bridge* bridge = nullptr;
    QString osName;
    int major = 0;
    int minor = 57;
    int patch = 8;
    int prerelease = 0;
};

Platform::Platform(QObject* parent) : QObject(parent), d_ptr(new PlatformPrivate) {}

Platform::~Platform() {}

void Platform::setBridge(Bridge* bridge) {
    Q_D(Platform);
    d->bridge = bridge;
}

QString Platform::moduleName() {
    return "RCTPlatformConstants";
}

QVariantMap Platform::constantsToExport() {
    Q_D(Platform);

    return QVariantMap{
        {"osName", d->osName},
        {"reactNativeVersion", QVariantMap{{"major", d->major}, {"minor", d->minor}, {"prerelease", d->prerelease}}}};
}
