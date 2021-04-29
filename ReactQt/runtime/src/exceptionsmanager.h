
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

#ifndef EXCEPTIONSMANAGER_H
#define EXCEPTIONSMANAGER_H

#include <QByteArray>
#include <QUrl>

#include "moduleinterface.h"

class QNetworkAccessManager;

class ExceptionsManagerPrivate;
class ExceptionsManager : public QObject, public ModuleInterface {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ModuleInterface)

    Q_INVOKABLE void reportSoftException(const QString& message, const QList<QVariantMap>& stack, int exceptionId);

    Q_INVOKABLE void reportFatalException(const QString& message, const QList<QVariantMap>& stack, int exceptionId);

    Q_INVOKABLE void updateExceptionMessage(const QString& message, const QList<QVariantMap>& stack, int exceptionId);

    Q_INVOKABLE void reportUnhandledException(const QString& message, const QList<QVariantMap>& stack);

    Q_DECLARE_PRIVATE(ExceptionsManager)

public:
    ExceptionsManager(QObject* parent = 0);
    ~ExceptionsManager();

    void setBridge(Bridge* bridge) override;

    // TODO: this doesnt seem right
    ViewManager* viewManager() override;

    QString moduleName() override;

private:
    QScopedPointer<ExceptionsManagerPrivate> d_ptr;
};

#endif // EXCEPTIONSMANAGER_H
