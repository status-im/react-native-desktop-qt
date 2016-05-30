
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


class QTcpSocket;

class ReactNetExecutor : public ReactExecutor
{
  Q_OBJECT

public:
  ReactNetExecutor(QObject* parent);
  ~ReactNetExecutor();

  void init() override;

  void injectJson(const QString& name, const QVariant& data) override;
  void executeApplicationScript(const QByteArray& script, const QUrl& sourceUrl) override;
  void executeJSCall(const QString& method,
                     const QVariantList& args = QVariantList(),
                     const ExecuteCallback& callback = ExecuteCallback()
                     ) override;

private Q_SLOTS:
  void readReply();

private:
  void sendRequest(const QByteArray& request);
  QJsonDocument handleResponse();
  void handleExecuteApplicationScriptResponse(const QVariantMap& message);
  void handleExecuteJavascriptCallResponse(const QVariantMap& message);

  QTcpSocket* m_socket;

  QByteArray m_inputBuffer;
};

#endif // REACTNETEXECUTOR_H
