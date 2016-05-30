
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

#ifndef REACTSOURCECODE_H
#define REACTSOURCECODE_H

#include <QByteArray>
#include <QUrl>

#include "reactmoduleinterface.h"


class QNetworkAccessManager;


class ReactSourceCode
  : public QObject
  , public ReactModuleInterface
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

  Q_PROPERTY(QUrl scriptURL READ scriptUrl CONSTANT)
  Q_PROPERTY(QByteArray sourceCode READ sourceCode NOTIFY sourceCodeChanged SCRIPTABLE false)

  Q_INVOKABLE void getScriptText(
      const ReactModuleInterface::ResponseBlock& success,
      const ReactModuleInterface::ErrorBlock& error);

public:
  ReactSourceCode(QObject* parent = 0);
  ~ReactSourceCode();

  void setBridge(ReactBridge* bridge) override;

  // TODO: this doesnt seem right
  ReactViewManager* viewManager() override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

  QUrl scriptUrl() const;
  void setScriptUrl(const QUrl& source);

  QByteArray sourceCode() const;

  void loadSource(QNetworkAccessManager* nam);

Q_SIGNALS:
  void scriptUrlChanged();
  void sourceCodeChanged();

private:
  ReactBridge* m_bridge;
  QUrl m_scriptUrl;
  QByteArray m_sourceCode;
};

#endif // REACTSOURCECODE_H
