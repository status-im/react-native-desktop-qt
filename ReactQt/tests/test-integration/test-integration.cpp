/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <QLoggingCategory>
#include <QSignalSpy>
#include <QTest>
#include <QTimer>
#include <QtQuick/QQuickView>

#include "bridge.h"
#include "reacttestcase.h"
#include "redbox.h"
#include "testmodule.h"

class TestIntegration : public ReactTestCase {
    Q_OBJECT

private slots:
    INIT_TEST_CASE_DEFAULT(ReactTestCase)
    CLEANUP_TEST_CASE_DEFAULT(ReactTestCase)
    CLEANUP_DEFAULT(ReactTestCase)
    INIT_DEFAULT(ReactTestCase)

    void testTestModuleMarkTestCompleted();
    void testJSExceptionReceived();
};

void TestIntegration::testTestModuleMarkTestCompleted() {
    loadQML(QUrl("qrc:/TestModuleTest.qml"));
    waitAndVerifyBridgeReady();

    TestModule* testModule = bridge()->testModule();
    QVERIFY(testModule);
    QSignalSpy spy(testModule, &TestModule::testCompleted);

    waitAndVerifyJsAppStarted();

    showView();
    waitAndVerifyCondition([&]() { return spy.count(); }, "Application running timeout");
}

void TestIntegration::testJSExceptionReceived() {
    loadJSBundle("TestJSException", "IntegrationTests/TestJSException");

    waitAndVerifyJsAppStarted();
    waitAndVerifyJSException("Error: Exception on componentDidMount. File TestJSException.js");
}

QTEST_MAIN(TestIntegration)
#include "test-integration.moc"
