
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef REACTCLIPBOARD_H
#define REACTCLIPBOARD_H

#include "moduleinterface.h"

class ClipboardPrivate;
class Clipboard : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_DECLARE_PRIVATE(Clipboard)

    Q_INVOKABLE REACT_PROMISE void getString(const ModuleInterface::ListArgumentBlock& resolve,
                                             const ModuleInterface::ListArgumentBlock& reject);
    Q_INVOKABLE void setString(const QString& clipboardContent);

public:
    Clipboard(QObject* parent = 0);
    virtual ~Clipboard();

    void setBridge(Bridge* bridge) override;

    QString moduleName() override;

private:
    QScopedPointer<ClipboardPrivate> d_ptr;
};

#endif // REACTCLIPBOARD_H
