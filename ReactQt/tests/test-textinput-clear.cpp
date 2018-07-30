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

class TestTextInputClear : public ReactTestCase {
    Q_OBJECT

private slots:

    virtual void initTestCase() override;
    CLEANUP_TEST_CASE_DEFAULT(ReactTestCase)
    CLEANUP_DEFAULT(ReactTestCase)
    INIT_DEFAULT(ReactTestCase)

    void testTextEmptyAfterClear();
};

void TestTextInputClear::initTestCase() {
    ReactTestCase::initTestCase();
    loadQML(QUrl("qrc:/TestTextInputClear.qml"));
    waitAndVerifyJsAppStarted();
    showView();
}

void TestTextInputClear::testTextEmptyAfterClear() {
    QQuickItem* topView = topJSComponent();
    QCOMPARE(valueOfControlProperty(topView, "objectName").toString(), QString("topView"));

    QQuickItem* textInput = topView->childItems().at(0);
    QCOMPARE(valueOfControlProperty(textInput, "objectName").toString(), QString("textInput"));

    QQuickItem* clearButton = topView->childItems().at(1);
    QCOMPARE(valueOfControlProperty(clearButton, "objectName").toString(), QString("clearButton"));

    QCOMPARE(valueOfControlProperty(textInput, "p_text").toString(), QString("Hello"));

    clickItem(clearButton);

    waitAndVerifyCondition([=]() { return valueOfControlProperty(textInput, "p_text").toString().isEmpty(); },
                           "TextInput not cleared");
}

QTEST_MAIN(TestTextInputClear)
#include "test-textinput-clear.moc"
