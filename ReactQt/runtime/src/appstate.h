
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

#ifndef APPSTATE_H
#define APPSTATE_H

#include <QByteArray>
#include <QUrl>

#include "moduleinterface.h"

class AppState : public QObject, public ModuleInterface {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ModuleInterface)

    Q_INVOKABLE void getCurrentAppState(const ModuleInterface::ListArgumentBlock& success,
                                        const ModuleInterface::MapArgumentBlock& error);

public:
    AppState(QObject* parent = 0);
    ~AppState();

    void setBridge(Bridge* bridge) override;

    // TODO: this doesnt seem right
    ViewManager* viewManager() override;

    QString moduleName() override;

private:
    Bridge* m_bridge;
};

#endif // APPSTATE_H
