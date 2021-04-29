/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <QSignalSpy>
#include <QTest>
#include <QtQuick/QQuickView>

#include "bridge.h"
#include "layout/flexbox.h"
#include "reacttestcase.h"
#include "redbox.h"
#include "testmodule.h"

class TestButtonSize : public ReactTestCase {
    Q_OBJECT

private slots:

    virtual void initTestCase() override;
    CLEANUP_TEST_CASE_DEFAULT(ReactTestCase)
    CLEANUP_DEFAULT(ReactTestCase)
    INIT_DEFAULT(ReactTestCase)

    void testButtonShouldBeVisible();
};

void TestButtonSize::initTestCase() {
    ReactTestCase::initTestCase();
    loadQML(QUrl("qrc:/TestButtonSize.qml"));
    waitAndVerifyJsAppStarted();
    showView();
}

void TestButtonSize::testButtonShouldBeVisible() {
    QQuickItem* topView = topJSComponent();
    QCOMPARE(valueOfControlProperty(topView, "objectName").toString(), QString("TopView"));

    QQuickItem* contentView = topView->childItems().at(0);
    QCOMPARE(valueOfControlProperty(contentView, "objectName").toString(), QString("Content"));

    QQuickItem* buttonView = contentView->childItems().at(0);
    QCOMPARE(valueOfControlProperty(buttonView, "objectName").toString(), QString("TestButton"));

    auto buttonFlexbox = Flexbox::findFlexbox(buttonView);
    Q_ASSERT(buttonFlexbox);
    waitAndVerifyCondition([=]() { return !buttonFlexbox->isDirty(); }, "Flexbox wasn't recalculated");

    QVERIFY2(buttonView->width() > 0, "Incorrect button width");
    QVERIFY2(buttonView->height() > 0, "Incorrect button height");
}

QTEST_MAIN(TestButtonSize)
#include "test-button-size.moc"
