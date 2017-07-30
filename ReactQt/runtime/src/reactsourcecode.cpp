
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

#include <memory>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>

#include "reactsourcecode.h"


class ReactSourceCodePrivate
{
public:
  ReactBridge* bridge = nullptr;
  QUrl scriptUrl;
  QByteArray sourceCode;
  int retryCount = 4;
  int retryAttempts = 0;
  int retryTimeout = 250;
};


void ReactSourceCode::getScriptText(
  const ReactModuleInterface::ListArgumentBlock& resolve,
  const ReactModuleInterface::ListArgumentBlock& reject
) {
  Q_D(ReactSourceCode);
  if (!d->sourceCode.isNull())
    resolve(d->bridge, QVariantList{QVariantMap{{"text", d->sourceCode},
                                                {"url", d->scriptUrl.toString()}}});
  else
    reject(d->bridge, QVariantList{QVariantMap{ {"text", "Source code is not available"}}});
}


ReactSourceCode::ReactSourceCode(QObject* parent)
  : QObject(parent)
  , d_ptr(new ReactSourceCodePrivate)
{
}

ReactSourceCode::~ReactSourceCode()
{
}


void ReactSourceCode::setBridge(ReactBridge* bridge)
{
  Q_D(ReactSourceCode);
  d->bridge = bridge;
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
  return d_func()->scriptUrl;
}

void ReactSourceCode::setScriptUrl(const QUrl& scriptUrl)
{
  Q_D(ReactSourceCode);
  if (d->scriptUrl == scriptUrl)
    return;
  d->scriptUrl = scriptUrl;
}

QByteArray ReactSourceCode::sourceCode() const
{
  return d_func()->sourceCode;
}

int ReactSourceCode::retryCount() const
{
  return d_func()->retryCount;
}

void ReactSourceCode::setRetryCount(int retryCount)
{
  Q_D(ReactSourceCode);
  if (d->retryCount == retryCount)
    return;
  d->retryCount = retryCount;
  Q_EMIT retryCountChanged();
}

void ReactSourceCode::loadSource(QNetworkAccessManager* nam)
{
  Q_D(ReactSourceCode);

  QNetworkRequest request(d->scriptUrl);
  QNetworkReply* reply = nam->get(request);
  QObject::connect(reply, &QNetworkReply::finished, [=]() {
      reply->deleteLater();
      if (reply->error() != QNetworkReply::NoError) {
        if (d->retryAttempts < d->retryCount) {
          d->retryAttempts++;
          d->retryTimeout *= 2;
          QTimer::singleShot(d->retryTimeout, [=] { loadSource(nam); });
        } else {
          qCritical() << __PRETTY_FUNCTION__ << ": Error while loading source" << reply->errorString();
          Q_EMIT loadFailed();
        }
        return;
      }
      d->sourceCode = reply->readAll();
      d->retryAttempts = 0;
      d->retryTimeout = 250;
      Q_EMIT sourceCodeChanged();
    });
}
