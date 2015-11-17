
#include <QQmlEngine>
#include <QTimer>

#include "reactview.h"


#include "reactbridge.h"


ReactView::ReactView(QQuickItem* parent)
  : QQuickItem(parent)
  , m_bridge(new ReactBridge(this))
{
  connect(m_bridge, SIGNAL(bridgeReady()), SLOT(bridgeReady()));
}

ReactView::~ReactView()
{
}

QString ReactView::moduleName() const
{
  return m_moduleName;
}

void ReactView::setModuleName(const QString& moduleName)
{
  if (m_moduleName != moduleName) {
    m_moduleName = moduleName;
    emit moduleNameChanged();
  }
}

QUrl ReactView::codeLocation() const
{
  return m_codeLocation;
}

void ReactView::setCodeLocation(const QUrl& codeLocation)
{
  if (m_codeLocation != codeLocation) {
    m_codeLocation = codeLocation;
    emit codeLocationChanged();
  }
}

QVariantMap ReactView::properties() const
{
  return m_properties;
}

void ReactView::setProperties(const QVariantMap& properties)
{
  m_properties = properties;
}

void ReactView::bridgeReady()
{
  QVariantList args;
  args.append(m_moduleName);

  QVariantMap params;
  params.insert("rootTag", 11); // TODO:
  params.insert("initialProps", m_properties);
  args.append(params);

  m_bridge->enqueueJSCall("AppRegistry", "runApplication", args);
}

void ReactView::componentComplete()
{
  QQuickItem::componentComplete();
  QTimer::singleShot(0, [=]() {
      // TODO: setQmlEngine && setNetworkAccessManager to be just setQmlEngine && then internal?
      m_bridge->setQmlEngine(qmlEngine(this));
      m_bridge->setNetworkAccessManager(qmlEngine(this)->networkAccessManager());
      m_bridge->setBundleUrl(m_codeLocation);
      m_bridge->setVisualParent(this);
      m_bridge->init();
    });
}
