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
#include <QDebug>
#include "reacttestcase.h"
#include "reactview.h"
#include "reactbridge.h"
#include "reactimagemanager.h"
#include "reactpropertyhandler.h"

class TestImageProps : public ReactTestCase {
  Q_OBJECT

private:
  QQuickItem* qmlImage();

private slots:

  void initTestCase();
  void cleanupTestCase();

  void checkTestIDProp();

private:
  ReactImageManager m_ImageManager;
};


QQuickItem* TestImageProps::qmlImage()
{
  //Even when in JS we have only one <Image> component returned in render(),
  //it is wrapped in <View> component implicitly, so we have hierarchy in QML:
  // ReactView
  //  |-<View>
  //    |-<Image>


  QList<QQuickItem*> reactViewChilds = rootView()->childItems();
  Q_ASSERT(reactViewChilds.count()==1);

  QQuickItem* view = reactViewChilds[0];
  QList<QQuickItem*> viewChilds = view->childItems();
  Q_ASSERT(viewChilds.count()==1);

  QQuickItem* image = viewChilds[0];
  Q_ASSERT(image);

  return image;
}


void TestImageProps::initTestCase()
{
  ReactTestCase::initTestCase();
  loadQML(QUrl("qrc:/TestImageProps.qml"));
  waitAndVerifyJsAppStarted();
}


void TestImageProps::cleanupTestCase()
{
  ReactTestCase::cleanupTestCase();
}


void TestImageProps::checkTestIDProp()
{
  //m_ImageManager.propertyHandler(qmlImage())->
  QString testID = qmlImage()->property("testID").toString();
  QCOMPARE(testID, QString("testImage"));
}

QTEST_MAIN(TestImageProps)
#include "test-image-props.moc"
