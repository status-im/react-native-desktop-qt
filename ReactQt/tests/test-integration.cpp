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

#include "reactattachedproperties.h"
#include "reactflexlayout.h"
#include "reacttextproperties.h"
#include "reactrawtextproperties.h"
#include "reactitem.h"
#include "reactview.h"
#include "reactbridge.h"
#include "reacttestmodule.h"

class TestIntegration : public QObject {
  Q_OBJECT

  QQuickView* m_quickView = nullptr;

private slots:

  void initTestCase();
  void cleanupTestCase();

  void testTestModuleMarkTestCompleted();
};

void registerTypes()
{
  qmlRegisterUncreatableType<ReactAttachedProperties>("React", 0, 1, "React", "React is not meant to be created directly");
  qmlRegisterUncreatableType<ReactFlexLayout>("React", 0, 1, "Flex", "Flex is not meant to be created directly");
  qmlRegisterUncreatableType<ReactTextProperties>("React", 0, 1, "Text", "Text is not meant to be created directly");
  qmlRegisterUncreatableType<ReactRawTextProperties>("React", 0, 1, "RawText", "Text is not meant to be created directly");
  qmlRegisterType<ReactItem>("React", 0, 1, "Item");
  qmlRegisterType<ReactView>("React", 0, 1, "RootView");
}

void TestIntegration::initTestCase()
{
  m_quickView = new QQuickView();

  registerTypes();
}

void TestIntegration::cleanupTestCase()
{
  if (m_quickView) {
    m_quickView->deleteLater();
    m_quickView = nullptr;
  }
}

void TestIntegration::testTestModuleMarkTestCompleted()
{
  m_quickView->setSource(QUrl("qrc:/TestModuleTest.qml"));
  while(m_quickView->status() == QQuickView::Loading) {
    QCoreApplication::processEvents();
  }

  QObject* root = m_quickView->rootObject();
  QVERIFY(root);

  QQuickItem* rootView = root->findChild<QQuickItem *>("rootView");
  QVERIFY(rootView);

  ReactView* reactView = qobject_cast<ReactView*>(rootView);
  QVERIFY(reactView);

  ReactBridge* reactBridge = reactView->bridge();
  QVERIFY(reactBridge);

  QTimer timeoutTimer;
  timeoutTimer.setSingleShot(true);
  timeoutTimer.setInterval(30000);
  timeoutTimer.start();

  while(!reactBridge->ready() && timeoutTimer.isActive()) {
    QCoreApplication::processEvents();
  }

  QVERIFY2(timeoutTimer.isActive(), "ReactBridge ready timeout");

  ReactTestModule* testModule = reactBridge->testModule();
  QVERIFY(testModule);
  QSignalSpy spy(testModule, &ReactTestModule::testCompleted);

  m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);
  m_quickView->show();

  timeoutTimer.stop();
  timeoutTimer.start();

  while(!spy.count() && timeoutTimer.isActive()) {
    QCoreApplication::processEvents();
  }

  QVERIFY2(timeoutTimer.isActive(), "Application running timeout");

  QVERIFY(spy.count());
}

QTEST_MAIN(TestIntegration)
#include "test-integration.moc"
