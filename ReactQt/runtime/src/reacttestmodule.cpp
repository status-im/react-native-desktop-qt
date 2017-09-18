/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "reacttestmodule.h"
#include "reactbridge.h"
#include "reacteventdispatcher.h"

#include <QDebug>

class ReactTestModulePrivate {
public:
    ReactBridge* bridge = nullptr;
};

ReactTestModule::ReactTestModule(QObject* parent) : QObject(parent), d_ptr(new ReactTestModulePrivate) {}

ReactTestModule::~ReactTestModule() {}

void ReactTestModule::setBridge(ReactBridge* bridge) {
    Q_D(ReactTestModule);
    d->bridge = bridge;
}

QString ReactTestModule::moduleName() {
    return "TestModule";
}

QList<ReactModuleMethod*> ReactTestModule::methodsToExport() {
    return QList<ReactModuleMethod*>{};
}

QVariantMap ReactTestModule::constantsToExport() {
    return QVariantMap{};
}

void ReactTestModule::markTestCompleted() {
    emit testCompleted();
}
