
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <QAbstractListModel>

#include "bridge.h"
#include "redbox.h"
#include "utilities.h"

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
} // namespace

class RedboxPrivate : public QAbstractListModel {

    Q_OBJECT
    Q_PROPERTY(bool empty READ empty NOTIFY emptyChanged)
    Q_DECLARE_PUBLIC(Redbox)

public:
    QList<QVariantMap> stack;
    QQuickItem* redbox = nullptr;
    Redbox* q_ptr = nullptr;
    Bridge* bridge = nullptr;

    RedboxPrivate(Redbox* q) : q_ptr(q) {}

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

    void createRedboxItem() {
        redbox = utilities::createQMLItemFromSourceFile(bridge->qmlEngine(), QUrl("qrc:/qml/ReactRedboxItem.qml"));
        if (redbox == nullptr) {
            qCritical() << __PRETTY_FUNCTION__ << "Unable to create RedboxItem";
            return;
        }

        redbox->setObjectName("Redbox");
        redbox->setProperty("stackModel", QVariant::fromValue(this));

        connect(redbox, SIGNAL(reloadPressed()), this, SLOT(reloadPressed()));
        connect(redbox, SIGNAL(dismissPressed()), this, SLOT(dismissPressed()));
    }

public Q_SLOTS:

    void reloadPressed() {
        redbox->setParentItem(0);
        bridge->reload();
    }

    void dismissPressed() {
        redbox->setParentItem(0);
    }

Q_SIGNALS:
    void emptyChanged();
};

Redbox::Redbox(Bridge* bridge) : QObject(0), d_ptr(new RedboxPrivate(this)) {
    Q_D(Redbox);
    d->bridge = bridge;
    d->createRedboxItem();
}

Redbox::~Redbox() {}

void Redbox::showErrorMessage(const QString& message, const QList<QVariantMap>& stack) {
    Q_D(Redbox);
    d->redbox->setProperty(REDBOX_MESSAGE_PROPERTY, message);
    d->setStack(stack);

    d->redbox->setParentItem(d->bridge->topmostVisualParent());
}

void Redbox::updateErrorMessage(const QString& message, const QList<QVariantMap>& stack) {
    Q_D(Redbox);
    d->redbox->setProperty(REDBOX_MESSAGE_PROPERTY, message);
    d->setStack(stack);
}

QString Redbox::errorMessage() const {
    return d_func()->redbox->property(REDBOX_MESSAGE_PROPERTY).toString();
}

#include "redbox.moc"
