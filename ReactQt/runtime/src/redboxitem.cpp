
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

#include <QAbstractListModel>

#include "bridge.h"
#include "redboxitem.h"

namespace {

enum Roles {
    LineNumber = Qt::UserRole + 1,
    Column,
    File,
    MethodName,
};

static QHash<int, QByteArray> roleData{
    {LineNumber, "lineNumber"}, {Column, "column"}, {File, "file"}, {MethodName, "methodName"},
};

const char REDBOX_MESSAGE_PROPERTY[] = "message";
}

class RedboxItemPrivate : public QAbstractListModel {

    Q_OBJECT
    Q_PROPERTY(bool empty READ empty NOTIFY emptyChanged)
    Q_DECLARE_PUBLIC(RedboxItem)

public:
    QList<QVariantMap> stack;
    QQuickItem* redbox;
    RedboxItem* q_ptr;
    Bridge* bridge;

    RedboxItemPrivate(RedboxItem* q) : q_ptr(q) {}

    void setStack(const QList<QVariantMap>& stack) {
        const bool oe = this->stack.isEmpty();
        beginResetModel();
        this->stack = stack;
        if (!stack.isEmpty()) {
            beginInsertRows(QModelIndex(), 0, stack.length());
            endInsertRows();
        }
        if (this->stack.isEmpty() != oe)
            Q_EMIT emptyChanged();
        endResetModel();
    }

    int rowCount(const QModelIndex& parent) const override {
        return stack.length();
    }

    QVariant data(const QModelIndex& index, int role) const override {
        return stack.at(index.row()).value(roleData[role]).toString();
    }

    QHash<int, QByteArray> roleNames() const override {
        return roleData;
    }

    bool empty() const {
        return stack.isEmpty();
    }

    void createRedboxItem(QQuickItem* parent) {

        QQmlComponent component(bridge->qmlEngine());
        component.loadUrl(QUrl::fromLocalFile(":/qml/ReactRedboxItem.qml"));
        redbox = qobject_cast<QQuickItem*>(component.create());
        if (redbox == nullptr) {
            qCritical() << __PRETTY_FUNCTION__ << "Unable to create RedboxItem" << component.errors();
            return;
        }

        redbox->setObjectName("Redbox");
        redbox->setParentItem(parent);
        redbox->setProperty("stackModel", QVariant::fromValue(this));

        connect(redbox, SIGNAL(reloadPressed()), this, SLOT(reloadPressed()));
        connect(redbox, SIGNAL(dismissPressed()), this, SLOT(dismissPressed()));
    }

public Q_SLOTS:

    void reloadPressed() {
        q_ptr->setParentItem(0);
        bridge->reload();
    }

    void dismissPressed() {
        q_ptr->setParentItem(0);
    }

Q_SIGNALS:
    void emptyChanged();
};

RedboxItem::RedboxItem(Bridge* bridge) : QQuickItem(0), d_ptr(new RedboxItemPrivate(this)) {
    Q_D(RedboxItem);
    d->bridge = bridge;
    d->createRedboxItem(this);
}

RedboxItem::~RedboxItem() {}

void RedboxItem::showErrorMessage(const QString& message, const QList<QVariantMap>& stack) {
    Q_D(RedboxItem);
    d->redbox->setProperty(REDBOX_MESSAGE_PROPERTY, message);
    d->setStack(stack);

    QQuickItem* rootView = d->bridge->visualParent();
    setParentItem(rootView);

    setX(0);
    setY(0);
    setWidth(rootView->width());
    setHeight(rootView->height());
}

void RedboxItem::updateErrorMessage(const QString& message, const QList<QVariantMap>& stack) {
    Q_D(RedboxItem);
    d->redbox->setProperty(REDBOX_MESSAGE_PROPERTY, message);
    d->setStack(stack);
}

QString RedboxItem::errorMessage() const {
    return d_func()->redbox->property(REDBOX_MESSAGE_PROPERTY).toString();
}

#include "redboxitem.moc"
