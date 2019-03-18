
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef REACTLINKINGMANAGER_H
#define REACTLINKINGMANAGER_H

#include "moduleinterface.h"

class LinkingManagerPrivate;
class LinkingManager : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_DECLARE_PRIVATE(LinkingManager)

    Q_INVOKABLE REACT_PROMISE void openURL(const QString& url,
                                           const ModuleInterface::ListArgumentBlock& resolve,
                                           const ModuleInterface::ListArgumentBlock& reject);

    Q_INVOKABLE REACT_PROMISE void canOpenURL(const QString& url,
                                              const ModuleInterface::ListArgumentBlock& resolve,
                                              const ModuleInterface::ListArgumentBlock& reject);

    Q_INVOKABLE REACT_PROMISE void getInitialURL(const ModuleInterface::ListArgumentBlock& resolve,
                                                 const ModuleInterface::ListArgumentBlock& reject);

public:
    LinkingManager(QObject* parent = 0);
    virtual ~LinkingManager();

    void setBridge(Bridge* bridge) override;

    QString moduleName() override;

private:
    QScopedPointer<LinkingManagerPrivate> d_ptr;
};

#endif // REACTLINKINGMANAGER_H
