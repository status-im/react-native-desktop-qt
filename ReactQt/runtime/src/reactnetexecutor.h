
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

#ifndef REACTNETEXECUTOR_H
#define REACTNETEXECUTOR_H

#include "reactexecutor.h"


class ReactNetExecutorPrivate;
class ReactNetExecutor : public ReactExecutor
{
  Q_OBJECT
  Q_PROPERTY(QString serverHost READ serverHost WRITE setServerHost)

  Q_DECLARE_PRIVATE(ReactNetExecutor)

public:
  Q_INVOKABLE ReactNetExecutor(QObject* parent = 0);
  ~ReactNetExecutor();

  QString serverHost() const;
  void setServerHost(const QString& serverHost);

  void init() override;

  void injectJson(const QString& name, const QVariant& data) override;
  void executeApplicationScript(const QByteArray& script, const QUrl& sourceUrl) override;
  void executeJSCall(const QString& method,
                     const QVariantList& args = QVariantList(),
                     const ExecuteCallback& callback = ExecuteCallback()) override;

private:
  void processRequest(const QByteArray& request, const ExecuteCallback& callback = ExecuteCallback());

  QScopedPointer<ReactNetExecutorPrivate> d_ptr;
};

#endif // REACTNETEXECUTOR_H
