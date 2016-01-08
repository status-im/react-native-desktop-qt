
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
      if (reply->error() != QNetworkReply::NoError) {
        qCritical() << __PRETTY_FUNCTION__ << ": Error while loading source" << reply->errorString();
        return;
      }
      m_sourceCode = reply->readAll();
      reply->deleteLater();
      Q_EMIT sourceCodeChanged();
    });
}
