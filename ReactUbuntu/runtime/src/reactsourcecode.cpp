
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

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "reactsourcecode.h"


void ReactSourceCode::getScriptText
(
 const ReactModuleInterface::ResponseBlock& success,
 const ReactModuleInterface::ErrorBlock& error
)
{
  if (!m_sourceCode.isNull())
    success(m_bridge, QVariantList{ QVariantMap{ {"text", m_sourceCode}, {"url", m_scriptUrl.toString()} } });
  else
    error(m_bridge, QVariantMap{ {"text", "Source code is not available"} });
}


ReactSourceCode::ReactSourceCode(QObject* parent)
  : QObject(parent)
{
}

ReactSourceCode::~ReactSourceCode()
{
}


void ReactSourceCode::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

ReactViewManager* ReactSourceCode::viewManager()
{
  return nullptr;
}

QString ReactSourceCode::moduleName()
{
  return "RCTSourceCode";
}

QList<ReactModuleMethod*> ReactSourceCode::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap ReactSourceCode::constantsToExport()
{
  return QVariantMap{};
}

QUrl ReactSourceCode::scriptUrl() const
{
  return m_scriptUrl;
}

void ReactSourceCode::setScriptUrl(const QUrl& scriptUrl)
{
  m_scriptUrl = scriptUrl;
}

QByteArray ReactSourceCode::sourceCode() const
{
  return m_sourceCode;
}

void ReactSourceCode::loadSource(QNetworkAccessManager* nam)
{
  QNetworkRequest request(m_scriptUrl);
  QNetworkReply* reply = nam->get(request);
  QObject::connect(reply, &QNetworkReply::finished, [=]() {
      reply->deleteLater();
      if (reply->error() != QNetworkReply::NoError) {
        qCritical() << __PRETTY_FUNCTION__ << ": Error while loading source" << reply->errorString();
        return;
      }
      m_sourceCode = reply->readAll();
      Q_EMIT sourceCodeChanged();
    });
}
