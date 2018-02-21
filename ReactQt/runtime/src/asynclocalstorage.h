
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

#ifndef ASYNCLOCALSTORAGE_H
#define ASYNCLOCALSTORAGE_H

#include <QByteArray>
#include <QUrl>

#include "moduleinterface.h"

class QSettings;

class AsyncLocalStorage : public QObject, public ModuleInterface {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ModuleInterface)

    Q_INVOKABLE void multiGet(const QList<QString>& keys, const ModuleInterface::ListArgumentBlock& callback);
    Q_INVOKABLE void multiSet(const QList<QList<QString>>& kvPairs, const ModuleInterface::ListArgumentBlock& callback);
    Q_INVOKABLE void multiRemove(const QList<QString>& keys, const ModuleInterface::ListArgumentBlock& callback);
    Q_INVOKABLE void clear(const ModuleInterface::ListArgumentBlock& callback);
    Q_INVOKABLE void getAllKeys(const ModuleInterface::ListArgumentBlock& callback);

public:
    AsyncLocalStorage(QObject* parent = 0);
    ~AsyncLocalStorage();

    void setBridge(Bridge* bridge) override;

    // TODO: this doesnt seem right
    ViewManager* viewManager() override;

    QString moduleName() override;

private:
    Bridge* m_bridge;
    QSettings* m_settings;
};

#endif // ASYNCLOCALSTORAGE_H
