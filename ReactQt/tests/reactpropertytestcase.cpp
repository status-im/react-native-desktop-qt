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
    return control()->property(propertyName.toStdString().c_str());
}

QQuickItem* ReactPropertyTestCase::singleControl() const {
    // Even when in JS we have only one <Image> component returned in render(),
    // it is wrapped in <View> component implicitly, so we have hierarchy in QML:
    // ReactView
    //  |-<View>
    //    |-<Image>

    QList<QQuickItem*> reactViewChilds = rootView()->childItems();
    Q_ASSERT(reactViewChilds.count() == 1);

    QQuickItem* view = reactViewChilds[0];
    QList<QQuickItem*> viewChilds = view->childItems();
    Q_ASSERT(viewChilds.count() == 1);

    QQuickItem* control = viewChilds[0];
    Q_ASSERT(control);

    return control;
}
