
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

#ifndef REACTVIEWMANAGER_H
#define REACTVIEWMANAGER_H

#include <QVariant>

#include "reactmoduleinterface.h"

class QQuickItem;

// #define QT_STATICPLUGIN

class ReactViewManager : public QObject, public ReactModuleInterface {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ReactModuleInterface)

public:
    ReactViewManager(QObject* parent = 0);
    ~ReactViewManager();

    virtual void setBridge(ReactBridge* bridge) override;

    // TODO: this doesnt seem right
    virtual ReactViewManager* viewManager() override;
    virtual ReactPropertyHandler* propertyHandler(QObject* object) override;
    virtual QString moduleName() override;

    virtual QList<ReactModuleMethod*> methodsToExport() override;
    virtual QVariantMap constantsToExport() override;
    virtual QStringList customDirectEventTypes();
    virtual QStringList customBubblingEventTypes();

    virtual bool shouldLayout() const;
    virtual void addChildItem(QQuickItem* parent, QQuickItem* child, int position) const;

    virtual QQuickItem* view(const QVariantMap& properties = QVariantMap()) const;

protected:
    QQuickItem* createView() const;
    ReactBridge* bridge() const;
    virtual void configureView(QQuickItem* view) const;
    virtual QString qmlComponentFile() const;
    void notifyJsAboutEvent(int senderTag, const QString& eventName, const QVariantMap& eventData) const;
    int tag(QQuickItem* view) const;

public slots:
    void manageTransformMatrix(const QVector<float>& transformMatrix, QQuickItem* object);

private:
    ReactBridge* m_bridge = nullptr;
};

#endif // REACTVIEWMANAGER_H
