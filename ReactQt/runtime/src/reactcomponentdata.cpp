
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

#include <QMetaObject>
#include <QMetaProperty>
#include <QQuickItem>

#include <QDebug>

#include "reactattachedproperties.h"
#include "reactcomponentdata.h"
#include "reactmoduledata.h"
#include "reactpropertyhandler.h"
#include "reactviewmanager.h"
#include "utilities.h"

using namespace utilities;

ReactComponentData::ReactComponentData(ReactModuleData* moduleData) : m_moduleData(moduleData) {
    m_moduleInterface = m_moduleData->viewManager();
}

ReactComponentData::~ReactComponentData() {}

QString ReactComponentData::name() const {
    QString mn = m_moduleInterface->viewManager()->moduleName();
    int mi = mn.indexOf("Manager");
    if (mi != -1)
        return mn.left(mi);

    return mn;
}

ReactViewManager* ReactComponentData::manager() const {
    return m_moduleInterface->viewManager();
}

QVariantMap ReactComponentData::viewConfig() const {
    // qDebug() << __PRETTY_FUNCTION__ << name();

    QVariantMap rc;

    // Create a temporary view to inspect, oh well
    QQuickItem* view = m_moduleInterface->viewManager()->view();
    if (view == nullptr) {
        qWarning() << name() << "has no view for inspecting!";
        return rc;
    }
    view->deleteLater();

    ReactPropertyHandler* ph = m_moduleInterface->propertyHandler(view);
    ph->deleteLater(); // XXX:

    QStringList directEvents;
    QStringList bubblingEvents;

    // {{{ propTypes
    QMap<QString, QMetaProperty> properties = ph->availableProperties();

    // XXX: sort out the callback events..
    QVariantMap propTypes;
    for (auto& propName : properties.keys()) {
        auto pName = propName;
        auto p = properties.value(propName);

        if (p.userType() == qMetaTypeId<ReactModuleInterface::BubblingEventBlock>()) {
            propTypes.insert(p.name(), "bool");
            bubblingEvents << p.name();
        } else if (p.userType() == qMetaTypeId<ReactModuleInterface::DirectEventBlock>()) {
            propTypes.insert(p.name(), "bool");
            directEvents << p.name();
        } else {
            propTypes.insert(pName, p.typeName());
        }
    }

    rc.insert("propTypes", propTypes);
    // }}}

    // Events
    directEvents << m_moduleInterface->viewManager()->customDirectEventTypes();
    QStringList dep;
    std::transform(directEvents.begin(), directEvents.end(), std::back_inserter(dep), [](const QString& name) {
        return normalizeInputEventName(name);
    });
    rc.insert("directEvents", dep);

    bubblingEvents << m_moduleInterface->viewManager()->customBubblingEventTypes();
    dep.clear();
    std::transform(bubblingEvents.begin(), bubblingEvents.end(), std::back_inserter(dep), [](const QString& name) {
        return normalizeInputEventName(name);
    });
    rc.insert("bubblingEvents", dep);

    return rc;
}

void ReactComponentData::attachReactProperties(QQuickItem* view, int tag) {
    auto viewManager = m_moduleInterface->viewManager();

    ReactAttachedProperties* rap = ReactAttachedProperties::get(view);
    rap->setTag(tag);
    rap->setShouldLayout(viewManager->shouldLayout());
    rap->setViewManager(viewManager);
    rap->setPropertyHandler(m_moduleInterface->propertyHandler(view));
}

QQuickItem* ReactComponentData::createView(int tag, const QVariantMap& properties) {
    QQuickItem* view = m_moduleInterface->viewManager()->view(properties);
    attachReactProperties(view, tag);
    return view;
}

ReactModuleMethod* ReactComponentData::method(int id) const {
    return m_moduleData->method(id);
}
