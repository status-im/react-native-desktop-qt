
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

#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include "rawtextmanager.h"

// #define QT_STATICPLUGIN

class TextManager : public RawTextManager {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ModuleInterface)

public:
    TextManager(QObject* parent = 0);
    ~TextManager();

    ViewManager* viewManager() override;
    PropertyHandler* propertyHandler(QObject* object) override;

    QString moduleName() override;

    bool shouldLayout() const override;

    typedef std::map<QString, QVariant> PropertyMap;
    static void resizeToWidth(QQuickItem* textItem, double width);
    static void resizeToHeight(QQuickItem* textItem, double height);

public slots:
    QVariant nestedPropertyValue(QQuickItem* item, const QString& propertyName);
    void updateMeasureFunction(QQuickItem* textItem);
    QString escape(const QString& text);

private:
    QQuickItem* parentTextItem(QQuickItem* textItem);
    bool propertyExplicitlySet(QQuickItem* item, const QString& propertyName);

    virtual QString qmlComponentFile(const QVariantMap& properties) const override;
    virtual void configureView(QQuickItem* view) const override;

private:
    using StringSet = QSet<QString>;
    using PropertiesMap = QMap<QObject*, StringSet>;

    PropertiesMap m_explicitlySetProps;
};

#endif // TEXTMANAGER_H
