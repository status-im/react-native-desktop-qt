
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


class ReactSourceCodePrivate;
class ReactSourceCode
  : public QObject
  , public ReactModuleInterface
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

  Q_PROPERTY(QUrl scriptURL READ scriptUrl CONSTANT)
  Q_PROPERTY(QByteArray sourceCode READ sourceCode NOTIFY sourceCodeChanged SCRIPTABLE false)
  Q_PROPERTY(int retryCount READ retryCount WRITE setRetryCount NOTIFY retryCountChanged SCRIPTABLE false)

  Q_INVOKABLE REACT_PROMISE
    void getScriptText(const ReactModuleInterface::ListArgumentBlock& resolve,
                       const ReactModuleInterface::ListArgumentBlock& reject);

  Q_DECLARE_PRIVATE(ReactSourceCode);

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

  int retryCount() const;
  void setRetryCount(int retryCount);

  void loadSource(QNetworkAccessManager* nam);

Q_SIGNALS:
  void scriptUrlChanged();
  void sourceCodeChanged();
  void retryCountChanged();
  void loadFailed();

private:
  QScopedPointer<ReactSourceCodePrivate> d_ptr;
};

#endif // REACTSOURCECODE_H
