
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

#include <QQuickItem>


class ReactBridge;


class ReactRedboxItemPrivate;
class ReactRedboxItem : public QQuickItem
{
  Q_DECLARE_PRIVATE(ReactRedboxItem)

public:
  ReactRedboxItem(ReactBridge* bridge);
  ~ReactRedboxItem();

  void showErrorMessage(const QString& message, const QList<QVariantMap>& stack = QList<QVariantMap>());
  void updateErrorMessage(const QString& message, const QList<QVariantMap>& stack = QList<QVariantMap>());

private:
  QScopedPointer<ReactRedboxItemPrivate> d_ptr;
};