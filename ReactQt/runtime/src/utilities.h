
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

#include <QMatrix4x4>
#include <QQuickTransform>
#include <QString>
#include <QUrl>

class QQuickItem;
class QQmlEngine;

namespace utilities {

const QString TOUCH_START = "touchStart";
const QString TOUCH_END = "touchEnd";
const QString TOUCH_MOVE = "touchMove";

class MatrixTransform : public QQuickTransform {
public:
    MatrixTransform(const QVector<float>& transformMatrix, QQuickItem* parent);
    void applyTo(QMatrix4x4* matrix) const override;
    QMatrix4x4 m_transformMatrix;
    QQuickItem* m_item;
};

using QmlComponentPtr = QSharedPointer<QQmlComponent>;

void registerReactTypes();
QString normalizeInputEventName(const QString& eventName);
QQuickItem* createQMLItemFromSourceFile(QQmlEngine* qmlEngine, const QUrl& fileUrl);
QmlComponentPtr createComponentFromSourceFile(QQmlEngine* qmlEngine, const QUrl& fileUrl);
QQuickItem* createQMLItemFromComponent(QmlComponentPtr component);
QObject* createQObjectInstance(const QString& typeName);
void insertChildItemAt(QQuickItem* item, int position, QQuickItem* parent);
void removeFlexboxChilds(QQuickItem* item, const QList<int>& removeAtIndices);
QVariantMap createTouchArgs(int tag, const QPointF& lp, const QPointF& local, const QString& button, ulong timestamp);
QQuickItem* getChildFromScrollView(QQuickItem* scrollView, const QPointF& scrollViewPos);
QQuickItem* getChildFromText(QQuickItem* text, const QPointF& pos);
QVariantMap makeReactTouchEvent(QQuickItem* item, QMouseEvent* event);

} // namespace utilities

#endif // UTILITIES
