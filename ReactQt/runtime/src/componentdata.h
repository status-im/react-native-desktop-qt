
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

#ifndef COMPONENTDATA_H
#define COMPONENTDATA_H

#include <QString>
#include <QVariant>

class QQuickItem;
class ModuleData;
class ViewManager;
class ModuleInterface;
class ModuleMethod;

class ComponentData {
public:
    ComponentData(ModuleData* moduleData);
    ~ComponentData();

    QString name() const;
    ViewManager* manager() const;

    QVariantMap viewConfig() const;

    QQuickItem* createView(int tag, const QVariantMap& properties);

    ModuleMethod* method(int id) const;

private:
    void attachReactProperties(QQuickItem* view, int tag);

private:
    ModuleData* m_moduleData;
    ModuleInterface* m_moduleInterface;
};

#endif // COMPONENTDATA_H
