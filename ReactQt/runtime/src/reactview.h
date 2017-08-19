
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

#ifndef REACTVIEW_H
#define REACTVIEW_H

#include <QString>
#include <QUrl>
#include <QScopedPointer>

#include "reactitem.h"


class ReactBridge;

class ReactViewPrivate;
class ReactView : public ReactItem
{
  Q_OBJECT

  Q_PROPERTY(bool liveReload READ liveReload WRITE setLiveReload NOTIFY liveReloadChanged)
  Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY moduleNameChanged)
  Q_PROPERTY(QUrl codeLocation READ codeLocation WRITE setCodeLocation NOTIFY codeLocationChanged)
  Q_PROPERTY(QVariantMap properties READ properties WRITE setProperties NOTIFY propertiesChanged)
  Q_PROPERTY(QString pluginsPath READ pluginsPath WRITE setPluginsPath NOTIFY pluginsPathChanged)
  Q_PROPERTY(QString executor READ executor WRITE setExecutor NOTIFY executorChanged)

  Q_DECLARE_PRIVATE(ReactView)

public:
  ReactView(QQuickItem *parent = 0);
  ~ReactView();

  bool liveReload() const;
  void setLiveReload(bool liveReload);

  QString moduleName() const;
  void setModuleName(const QString& moduleName);

  QUrl codeLocation() const;
  void setCodeLocation(const QUrl& codeLocation);

  QVariantMap properties() const;
  void setProperties(const QVariantMap& properties);

  QString pluginsPath() const;
  void setPluginsPath(const QString& pluginsPath);

  QString executor() const;
  void setExecutor(const QString& executor);

  ReactBridge* bridge() const;

Q_SIGNALS:
  void liveReloadChanged();
  void moduleNameChanged();
  void codeLocationChanged();
  void propertiesChanged();
  void pluginsPathChanged();
  void executorChanged();

private Q_SLOTS:
  void bridgeReady();

private:
  void componentComplete() override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  bool childMouseEventFilter(QQuickItem* item, QEvent* event) override;

  QScopedPointer<ReactViewPrivate> d_ptr;
};

#endif // REACTVIEW_H
