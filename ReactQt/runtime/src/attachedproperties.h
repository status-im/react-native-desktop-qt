
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

#ifndef ATTACHEDPROPERTIES_H
#define ATTACHEDPROPERTIES_H

#include <QObject>
#include <QScopedPointer>
#include <QtQml>

class QQuickItem;
class ViewManager;
class PropertyHandler;

class AttachedPropertiesPrivate;
class AttachedProperties : public QObject {
    Q_OBJECT
    Q_PROPERTY(int tag READ tag WRITE setTag)
    Q_PROPERTY(bool shouldLayout READ shouldLayout WRITE setShouldLayout)
    Q_PROPERTY(ViewManager* viewManager READ viewManager WRITE setViewManager)
    Q_PROPERTY(PropertyHandler* propertyHandler READ propertyHandler WRITE setPropertyHandler)

    Q_DECLARE_PRIVATE(AttachedProperties)

public:
    AttachedProperties(QObject* parent = 0);
    ~AttachedProperties();

    int tag() const;
    void setTag(int tag);

    bool shouldLayout() const;
    void setShouldLayout(bool shouldLayout);

    ViewManager* viewManager() const;
    void setViewManager(ViewManager* viewManager);

    PropertyHandler* propertyHandler() const;
    void setPropertyHandler(PropertyHandler* propertyHandler);

    void applyProperties(const QVariantMap& properties) const;

    static AttachedProperties* get(QQuickItem* item, bool create = true);
    static AttachedProperties* qmlAttachedProperties(QObject* object);

private:
    QScopedPointer<AttachedPropertiesPrivate> d_ptr;
};

QML_DECLARE_TYPEINFO(AttachedProperties, QML_HAS_ATTACHED_PROPERTIES)

#endif // ATTACHEDPROPERTIES_H
