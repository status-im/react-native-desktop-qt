
/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * Author: Justin McPherson <justin.mcpherson@canonical.com>
 *
 */

#ifndef REACTATTACHEDPROPERTIES_H
#define REACTATTACHEDPROPERTIES_H

#include <QObject>
#include <QScopedPointer>
#include <QtQml>

class QQuickItem;
class ReactViewManager;
class ReactPropertyHandler;

class ReactAttachedPropertiesPrivate;
class ReactAttachedProperties : public QObject {
    Q_OBJECT
    Q_PROPERTY(int tag READ tag WRITE setTag)
    Q_PROPERTY(bool shouldLayout READ shouldLayout WRITE setShouldLayout)
    Q_PROPERTY(ReactViewManager* viewManager READ viewManager WRITE setViewManager)
    Q_PROPERTY(ReactPropertyHandler* propertyHandler READ propertyHandler WRITE setPropertyHandler)

    Q_DECLARE_PRIVATE(ReactAttachedProperties)

public:
    ReactAttachedProperties(QObject* parent = 0);
    ~ReactAttachedProperties();

    int tag() const;
    void setTag(int tag);

    bool shouldLayout() const;
    void setShouldLayout(bool shouldLayout);

    ReactViewManager* viewManager() const;
    void setViewManager(ReactViewManager* viewManager);

    ReactPropertyHandler* propertyHandler() const;
    void setPropertyHandler(ReactPropertyHandler* propertyHandler);

    void applyProperties(const QVariantMap& properties) const;

    static ReactAttachedProperties* get(QQuickItem* item, bool create = true);
    static ReactAttachedProperties* qmlAttachedProperties(QObject* object);

private:
    QScopedPointer<ReactAttachedPropertiesPrivate> d_ptr;
};

QML_DECLARE_TYPEINFO(ReactAttachedProperties, QML_HAS_ATTACHED_PROPERTIES)

#endif // REACTATTACHEDPROPERTIES_H
