#ifndef REACTPROPERTYTTESTCASE_H
#define REACTPROPERTYTTESTCASE_H

#include "reacttestcase.h"
#include <QVariant>

class ReactPropertyTestCase : public ReactTestCase {
    Q_OBJECT
public:
    explicit ReactPropertyTestCase(QObject* parent = nullptr);

private slots:

    void testProperties();

protected:
    virtual QVariantMap propValues() const = 0;
    virtual QQuickItem* control() const = 0;
    QVariant valueOfProperty(const QString& propertyName);
};

#endif // REACTPROPERTYTTESTCASE_H
