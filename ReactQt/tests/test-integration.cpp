/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <QTest>
#include <QtQuick/QQuickView>
#include <QSignalSpy>

#include "reacttestcase.h"
#include "reacttestmodule.h"
#include "reactbridge.h"

class TestIntegration : public ReactTestCase {
  Q_OBJECT


private slots:

  void initTestCase();
  void cleanupTestCase();

  void testTestModuleMarkTestCompleted();
};


void TestIntegration::initTestCase()
{
  ReactTestCase::initTestCase();
  loadQML(QUrl("qrc:/TestModuleTest.qml"));
  waitAndVerifyBridgeReady();
}


void TestIntegration::cleanupTestCase()
{
  ReactTestCase::cleanupTestCase();
}

void TestIntegration::testTestModuleMarkTestCompleted()
{
  QTimer timeoutTimer;
  timeoutTimer.setSingleShot(true);
  timeoutTimer.setInterval(30000);

  ReactTestModule* testModule = bridge()->testModule();
  QVERIFY(testModule);
  QSignalSpy spy(testModule, &ReactTestModule::testCompleted);

  showView();
  waitAndVerifyCondition([&](){ qDebug()<<"spycount="<<spy.count(); return spy.count();}, "Application running timeout");
}

QTEST_MAIN(TestIntegration)
#include "test-integration.moc"
