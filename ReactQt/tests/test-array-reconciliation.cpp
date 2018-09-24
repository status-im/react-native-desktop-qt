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
#include <QTimer>
#include <QtQuick/QQuickView>

#include "bridge.h"
#include "reacttestcase.h"
#include "redbox.h"

class TestArrayReconciliation : public ReactTestCase {
    Q_OBJECT

private slots:
    virtual void initTestCase() override;
    CLEANUP_TEST_CASE_DEFAULT(ReactTestCase)
    CLEANUP_DEFAULT(ReactTestCase)
    INIT_DEFAULT(ReactTestCase)

    void testComponentsArrayFirstElementInsert();
    void testComponentsArrayLastElementDelete();
    void testComponentsArrayItemMove();

private:
    QQuickItem* topView = nullptr;
    const int INITIAL_ITEMS_COUNT = 4;
    const int CLICKED_ITEMS_COUNT = 3;

    void validateComponentsCount(const int expectedItemsCount, const QString& errorMsg);
};

void TestArrayReconciliation::initTestCase() {
    ReactTestCase::initTestCase();

    loadQML(QUrl("qrc:/TestArrayReconciliation.qml"));
    waitAndVerifyJsAppStarted();
    showView();
}

void TestArrayReconciliation::validateComponentsCount(const int expectedItemsCount, const QString& errorMsg) {
    clickDelayTimer->start();
    waitAndVerifyCondition([=]() { return !clickDelayTimer->isActive(); }, "Timer timeout was not triggered");
    waitAndVerifyCondition([=]() { return topView->childItems().size() == expectedItemsCount; }, errorMsg);
}

void TestArrayReconciliation::testComponentsArrayFirstElementInsert() {
    topView = topJSComponent();
    QCOMPARE(valueOfControlProperty(topView, "objectName").toString(), QString("topView"));

    QCOMPARE(topView->childItems().size(), INITIAL_ITEMS_COUNT);

    QCOMPARE(valueOfControlProperty(topView->childItems().at(1), "objectName").toString(), QString("FirstButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(2), "objectName").toString(), QString("SecondButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(3), "objectName").toString(), QString("ThirdButton"));

    QQuickItem* clickable = topView->childItems().at(0);
    clickItem(clickable);

    validateComponentsCount(CLICKED_ITEMS_COUNT, "Wrong array items count after 1st click");

    clickable = topView->childItems().at(0);
    clickItem(clickable);

    validateComponentsCount(INITIAL_ITEMS_COUNT, "Wrong array items count after 2nd click");

    clickable = topView->childItems().at(0);
    clickItem(clickable);

    validateComponentsCount(CLICKED_ITEMS_COUNT, "Wrong array items count after 3rd click");

    clickable = topView->childItems().at(0);
    clickItem(clickable);

    validateComponentsCount(INITIAL_ITEMS_COUNT, "Wrong array items count after 4th click");

    QCOMPARE(valueOfControlProperty(topView->childItems().at(1), "objectName").toString(), QString("FirstButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(2), "objectName").toString(), QString("SecondButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(3), "objectName").toString(), QString("ThirdButton"));
}

void TestArrayReconciliation::testComponentsArrayLastElementDelete() {
    loadJSBundle("TestArrayReconciliationDeleteLast", "ReactQt/tests/JS/TestArrayReconciliationDeleteLast");

    waitAndVerifyJsAppStarted();
    showView();

    clickDelayTimer->start();
    waitAndVerifyCondition([=]() { return !clickDelayTimer->isActive(); }, "Timer timeout was not triggered");

    topView = topJSComponent();
    QCOMPARE(valueOfControlProperty(topView, "objectName").toString(), QString("topView"));

    QCOMPARE(topView->childItems().size(), INITIAL_ITEMS_COUNT);

    QCOMPARE(valueOfControlProperty(topView->childItems().at(1), "objectName").toString(), QString("FirstButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(2), "objectName").toString(), QString("SecondButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(3), "objectName").toString(), QString("ThirdButton"));

    QQuickItem* clickable = topView->childItems().at(0);
    clickItem(clickable);

    validateComponentsCount(CLICKED_ITEMS_COUNT, "Wrong array items count after 1st click");

    clickable = topView->childItems().at(0);
    clickItem(clickable);

    validateComponentsCount(INITIAL_ITEMS_COUNT, "Wrong array items count after 2nd click");

    clickable = topView->childItems().at(0);
    clickItem(clickable);

    validateComponentsCount(CLICKED_ITEMS_COUNT, "Wrong array items count after 3rd click");

    clickable = topView->childItems().at(0);
    clickItem(clickable);

    validateComponentsCount(INITIAL_ITEMS_COUNT, "Wrong array items count after 4th click");

    QCOMPARE(valueOfControlProperty(topView->childItems().at(1), "objectName").toString(), QString("FirstButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(2), "objectName").toString(), QString("SecondButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(3), "objectName").toString(), QString("ThirdButton"));
}

void TestArrayReconciliation::testComponentsArrayItemMove() {
    const int ARRAY_ITEMS_COUNT = 4 + 1;

    loadJSBundle("TestArrayReconciliationItemMove", "ReactQt/tests/JS/TestArrayReconciliationItemMove");

    waitAndVerifyJsAppStarted();
    showView();

    clickDelayTimer->start();
    waitAndVerifyCondition([=]() { return !clickDelayTimer->isActive(); }, "Timer timeout was not triggered");

    topView = topJSComponent();
    QCOMPARE(valueOfControlProperty(topView, "objectName").toString(), QString("topView"));

    QCOMPARE(topView->childItems().size(), ARRAY_ITEMS_COUNT);

    QCOMPARE(valueOfControlProperty(topView->childItems().at(1), "objectName").toString(), QString("FirstButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(2), "objectName").toString(), QString("SecondButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(3), "objectName").toString(), QString("ThirdButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(4), "objectName").toString(), QString("FourthButton"));

    QQuickItem* clickable = topView->childItems().at(0);
    clickItem(clickable);

    validateComponentsCount(ARRAY_ITEMS_COUNT, "Wrong array items count after 1st click");

    QCOMPARE(valueOfControlProperty(topView->childItems().at(1), "objectName").toString(), QString("SecondButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(2), "objectName").toString(), QString("ThirdButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(3), "objectName").toString(), QString("FourthButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(4), "objectName").toString(), QString("FirstButton"));

    clickable = topView->childItems().at(0);
    clickItem(clickable);

    validateComponentsCount(ARRAY_ITEMS_COUNT, "Wrong array items count after 2nd click");

    QCOMPARE(valueOfControlProperty(topView->childItems().at(1), "objectName").toString(), QString("ThirdButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(2), "objectName").toString(), QString("FourthButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(3), "objectName").toString(), QString("FirstButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(4), "objectName").toString(), QString("SecondButton"));

    clickable = topView->childItems().at(0);
    clickItem(clickable);

    validateComponentsCount(ARRAY_ITEMS_COUNT, "Wrong array items count after 3rd click");

    QCOMPARE(valueOfControlProperty(topView->childItems().at(1), "objectName").toString(), QString("FourthButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(2), "objectName").toString(), QString("FirstButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(3), "objectName").toString(), QString("SecondButton"));
    QCOMPARE(valueOfControlProperty(topView->childItems().at(4), "objectName").toString(), QString("ThirdButton"));
}

QTEST_MAIN(TestArrayReconciliation)
#include "test-array-reconciliation.moc"
