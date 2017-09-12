
/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * Author: Justin McPherson <justin.mcpherson@canonical.com>
 *
 */

#ifndef REACTEXCEPTIONSMANAGER_H
#define REACTEXCEPTIONSMANAGER_H

#include <QByteArray>
#include <QUrl>

#include "reactmoduleinterface.h"

class QNetworkAccessManager;

class ReactExceptionsManagerPrivate;
class ReactExceptionsManager : public QObject, public ReactModuleInterface {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ReactModuleInterface)

    Q_INVOKABLE void reportSoftException(const QString& message, const QList<QVariantMap>& stack, int exceptionId);

    Q_INVOKABLE void reportFatalException(const QString& message, const QList<QVariantMap>& stack, int exceptionId);

    Q_INVOKABLE void updateExceptionMessage(const QString& message, const QList<QVariantMap>& stack, int exceptionId);

    Q_INVOKABLE void reportUnhandledException(const QString& message, const QList<QVariantMap>& stack);

    Q_DECLARE_PRIVATE(ReactExceptionsManager)

public:
    ReactExceptionsManager(QObject* parent = 0);
    ~ReactExceptionsManager();

    void setBridge(ReactBridge* bridge) override;

    // TODO: this doesnt seem right
    ReactViewManager* viewManager() override;

    QString moduleName() override;
    QList<ReactModuleMethod*> methodsToExport() override;
    QVariantMap constantsToExport() override;

private:
    QScopedPointer<ReactExceptionsManagerPrivate> d_ptr;
};

#endif // REACTEXCEPTIONSMANAGER_H
