
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef UTILITIES
#define UTILITIES

#include <QString>
#include <QUrl>

class QQuickItem;
class QQmlEngine;

namespace utilities {

void registerReactTypes();
QString normalizeInputEventName(const QString& eventName);
QQuickItem* createQMLItemFromSourceFile(QQmlEngine* qmlEngine, const QUrl& fileUrl);
QObject* createQObjectInstance(const QString& typeName);
void insertChildItemAt(QQuickItem* item, int position, QQuickItem* parent);

} // namespace utilities

#endif // UTILITIES
