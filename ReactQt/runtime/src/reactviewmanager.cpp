
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

#include <QMatrix4x4>
#include <QQmlProperty>
#include <QQuickItem>
#include <QString>
#include <QVariant>

#include "layout/flexbox.h"
#include "reactattachedproperties.h"
#include "reactbridge.h"
#include "reactitem.h"
#include "reactpropertyhandler.h"
#include "reacttextmanager.h"
#include "reactvaluecoercion.h"
#include "reactviewmanager.h"
#include "utilities.h"

using namespace utilities;

class MatrixTransform : public QQuickTransform {
    Q_OBJECT
public:
    MatrixTransform(const QVector<float>& transformMatrix, QQuickItem* parent)
        : QQuickTransform(parent), m_item(qobject_cast<QQuickItem*>(parent)) {
        memcpy(m_transformMatrix.data(), transformMatrix.constData(), 16 * sizeof(float));
        m_transformMatrix.optimize();
    }
    void applyTo(QMatrix4x4* matrix) const override {
        if (m_transformMatrix.isIdentity())
            return;
        matrix->translate(m_item->width() / 2, m_item->height() / 2);
        *matrix *= m_transformMatrix;
        matrix->translate(-m_item->width() / 2, -m_item->height() / 2);
    }
    QMatrix4x4 m_transformMatrix;
    QQuickItem* m_item;
};

ReactViewManager::ReactViewManager(QObject* parent) : QObject(parent) {}

ReactViewManager::~ReactViewManager() {}

void ReactViewManager::setBridge(ReactBridge* bridge) {
    m_bridge = bridge;
}

// TODO: this doesnt seem right
ReactViewManager* ReactViewManager::viewManager() {
    return this;
}

ReactPropertyHandler* ReactViewManager::propertyHandler(QObject* object) {
    return new ReactPropertyHandler(object);
}

QString ReactViewManager::moduleName() {
    return "RCTViewManager";
}

QList<ReactModuleMethod*> ReactViewManager::methodsToExport() {
    return QList<ReactModuleMethod*>{};
}

QVariantMap ReactViewManager::constantsToExport() {
    return QVariantMap{};
}

QStringList ReactViewManager::customDirectEventTypes() {
    return QStringList{};
}

QStringList ReactViewManager::customBubblingEventTypes() {
    return QStringList{"press",
                       "change",
                       "focus",
                       "blur",
                       "submitEditing",
                       "endEditing",
                       "touchStart",
                       "touchMove",
                       "touchCancel",
                       "touchEnd"};
}

bool ReactViewManager::shouldLayout() const {
    return true;
}

void ReactViewManager::addChildItem(QQuickItem* container, QQuickItem* child, int position) const {
    child->setParentItem(container);
}

QQuickItem* ReactViewManager::view(const QVariantMap& properties) const {
    QQuickItem* newView = createView();
    if (newView) {
        configureView(newView);
    }
    return newView;
}

void ReactViewManager::configureView(QQuickItem* view) const {
    view->setProperty("viewManager", QVariant::fromValue((QObject*)this));
}

QString ReactViewManager::qmlComponentFile() const {
    return ":/qml/ReactView.qml";
}

void ReactViewManager::notifyJsAboutEvent(int senderTag, const QString& eventName, const QVariantMap& eventData) const {
    bridge()->enqueueJSCall(
        "RCTEventEmitter", "receiveEvent", QVariantList{senderTag, normalizeInputEventName(eventName), eventData});
}

int ReactViewManager::tag(QQuickItem* view) const {
    ReactAttachedProperties* rap = ReactAttachedProperties::get(view, false);
    if (rap == nullptr) {
        qCritical() << "Could not get reacTag for view!";
        return -1;
    }
    Q_ASSERT(rap);
    return rap->tag();
}

QQuickItem* ReactViewManager::createView() const {
    QQmlComponent component(m_bridge->qmlEngine());
    auto file = qmlComponentFile();
    component.loadUrl(QUrl::fromLocalFile(file));
    if (!component.isReady()) {
        qCritical() << QString("Component for %1 is not ready!").arg(qmlComponentFile()) << component.errors();
        return nullptr;
    }

    QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
    if (item == nullptr) {
        qCritical() << QString("Unable to construct item from component %1").arg(qmlComponentFile());
    }
    return item;
}

ReactBridge* ReactViewManager::bridge() const {
    Q_ASSERT(m_bridge);
    return m_bridge;
}

void ReactViewManager::manageTransformMatrix(const QVector<float>& transformMatrix, QQuickItem* object) {
    QQmlListReference r(object, "transform");
    r.clear();
    r.append(new MatrixTransform(transformMatrix, qobject_cast<QQuickItem*>(object)));
}

#include "reactviewmanager.moc"
