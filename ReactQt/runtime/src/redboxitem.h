
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

#ifndef REDBOXITEM_H
#define REDBOXITEM_H

#include <QQuickItem>

class Bridge;

class RedboxItemPrivate;
class RedboxItem : public QQuickItem {
    Q_DECLARE_PRIVATE(RedboxItem)

public:
    RedboxItem(Bridge* bridge);
    ~RedboxItem();

    void showErrorMessage(const QString& message, const QList<QVariantMap>& stack = QList<QVariantMap>());
    void updateErrorMessage(const QString& message, const QList<QVariantMap>& stack = QList<QVariantMap>());

    QString errorMessage() const;

private:
    QScopedPointer<RedboxItemPrivate> d_ptr;
};

#endif // REDBOXITEM_H
