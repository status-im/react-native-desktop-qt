/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "testmodule.h"
#include "bridge.h"
#include "eventdispatcher.h"

#include <QDebug>

class TestModulePrivate {
public:
    Bridge* bridge = nullptr;
};

TestModule::TestModule(QObject* parent) : QObject(parent), d_ptr(new TestModulePrivate) {}

TestModule::~TestModule() {}

void TestModule::setBridge(Bridge* bridge) {
    Q_D(TestModule);
    d->bridge = bridge;
}

QString TestModule::moduleName() {
    return "TestModule";
}

void TestModule::markTestCompleted() {
    emit testCompleted();
}
