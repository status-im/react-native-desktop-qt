#include "reacttestcase.h"

#include "reactpropertytestcase.h"
#include "rootview.h"
#include <QQuickItem>
#include <QTest>

ReactPropertyTestCase::ReactPropertyTestCase(QObject* parent) : ReactTestCase(parent) {}

void ReactPropertyTestCase::testProperties() {
    for (const QString& propertyName : propValues().keys()) {
        auto propertyValue = propValues()[propertyName];
        QCOMPARE(valueOfProperty(propertyName), propertyValue);
    }
}

QVariant ReactPropertyTestCase::valueOfProperty(const QString& propertyName) {
    return valueOfControlProperty(control(), propertyName);
}
