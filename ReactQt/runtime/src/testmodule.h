/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef TESTMODULE_H
#define TESTMODULE_H

#include "moduleinterface.h"

#include <QVariantMap>

class TestModulePrivate;
class TestModule : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)

    Q_DECLARE_PRIVATE(TestModule)

public:
    TestModule(QObject* parent = 0);
    ~TestModule();

    void setBridge(Bridge* bridge) override;

    QString moduleName() override;

    Q_INVOKABLE void markTestCompleted();

signals:
    void testCompleted();

private:
    QScopedPointer<TestModulePrivate> d_ptr;
};

#endif // TESTMODULE_H
