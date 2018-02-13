
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef REDBOX_H
#define REDBOX_H

#include <QQuickItem>

class Bridge;

class RedboxPrivate;
class Redbox : public QObject {
    Q_DECLARE_PRIVATE(Redbox)

public:
    Redbox(Bridge* bridge);
    ~Redbox();

    void showErrorMessage(const QString& message, const QList<QVariantMap>& stack = QList<QVariantMap>());
    void updateErrorMessage(const QString& message, const QList<QVariantMap>& stack = QList<QVariantMap>());

    QString errorMessage() const;

private:
    QScopedPointer<RedboxPrivate> d_ptr;
};

#endif // REDBOX_H
