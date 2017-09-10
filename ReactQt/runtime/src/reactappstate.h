
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

#ifndef REACTAPPSTATE_H
#define REACTAPPSTATE_H

#include <QByteArray>
#include <QUrl>

#include "reactmoduleinterface.h"

class ReactAppState : public QObject, public ReactModuleInterface {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ReactModuleInterface)

    Q_INVOKABLE void getCurrentAppState(const ReactModuleInterface::ListArgumentBlock& success,
                                        const ReactModuleInterface::MapArgumentBlock& error);

public:
    ReactAppState(QObject* parent = 0);
    ~ReactAppState();

    void setBridge(ReactBridge* bridge) override;

    // TODO: this doesnt seem right
    ReactViewManager* viewManager() override;

    QString moduleName() override;
    QList<ReactModuleMethod*> methodsToExport() override;
    QVariantMap constantsToExport() override;

private:
    ReactBridge* m_bridge;
};

#endif // REACTAPPSTATE_H
