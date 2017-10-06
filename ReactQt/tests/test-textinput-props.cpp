/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "reactpropertytestcase.h"

#include <QDebug>
#include <QTest>
#include <QtQuick/QQuickView>

class TestTextInputProps : public ReactPropertyTestCase {
    Q_OBJECT

private slots:

    virtual void initTestCase() override;

protected:
    virtual QQuickItem* control() const override;
    virtual QVariantMap propValues() const override;
};

QQuickItem* TestTextInputProps::control() const {
    return topJSComponent();
}

void TestTextInputProps::initTestCase() {
    ReactPropertyTestCase::initTestCase();
    loadQML(QUrl("qrc:/TestTextInputProps.qml"));
    waitAndVerifyJsAppStarted();
}

QVariantMap TestTextInputProps::propValues() const {
    return {{"p_testID", "textinput"},
            {"p_text", "some text"},
            {"p_textAlign", "left"},
            {"p_multiline", true},
            {"p_onChange", true},
            {"p_selectionColor", "red"},
            {"p_onSelectionChange", true},
            {"p_color", QColor("green")},
            {"p_placeholderText", "Enter text to see events"}};
}

QTEST_MAIN(TestTextInputProps)
#include "test-textinput-props.moc"
