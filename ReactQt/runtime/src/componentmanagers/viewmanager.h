
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

#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <QVariant>

#include "moduleinterface.h"

class QQuickItem;

// #define QT_STATICPLUGIN

class ViewManager : public QObject, public ModuleInterface {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ModuleInterface)

public:
    ViewManager(QObject* parent = 0);
    ~ViewManager();

    virtual void setBridge(Bridge* bridge) override;

    // TODO: this doesnt seem right
    virtual ViewManager* viewManager() override;
    virtual PropertyHandler* propertyHandler(QObject* object) override;
    virtual QString moduleName() override;

    virtual QList<ModuleMethod*> methodsToExport() override;
    virtual QVariantMap constantsToExport() override;
    virtual QStringList customDirectEventTypes();
    virtual QStringList customBubblingEventTypes();

    virtual bool shouldLayout() const;
    virtual void addChildItem(QQuickItem* parent, QQuickItem* child, int position) const;

    virtual QQuickItem* view(const QVariantMap& properties = QVariantMap()) const;
    static int tag(QQuickItem* view);

protected:
    QQuickItem* createView() const;
    Bridge* bridge() const;
    virtual void configureView(QQuickItem* view) const;
    virtual QString qmlComponentFile() const;
    void notifyJsAboutEvent(int senderTag, const QString& eventName, const QVariantMap& eventData) const;

public slots:
    void manageTransformMatrix(const QVector<float>& transformMatrix, QQuickItem* object);

private:
    Bridge* m_bridge = nullptr;
};

#endif // VIEWMANAGER_H
