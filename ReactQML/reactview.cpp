
#include <QQmlEngine>
#include <QTimer>


#include "reactbridge.h"
#include "ubuntuuimanager.h"
#include "reactattachedproperties.h"
#include "reactview.h"
#include "reactflexlayout.h"

ReactView::ReactView(QQuickItem* parent)
  : ReactItem(parent)
  , m_bridge(new ReactBridge(this))
{
  setAcceptedMouseButtons(Qt::LeftButton);
  setFiltersChildMouseEvents(true);
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
  // XXX: should do the root view tag allocation internally
  ReactAttachedProperties* properties = ReactAttachedProperties::get(this);
  properties->setTag(m_bridge->uiManager()->allocateRootTag());
  m_bridge->uiManager()->registerRootView(this);

  QVariantList args{
    m_moduleName,
    QVariantMap{
      { "rootTag", properties->tag() },
      { "initialProps", m_properties }
    }
  };

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

void printDetails(QQuickItem* item, const QString& indent)
{
  qDebug().noquote() << indent << ReactAttachedProperties::get(item)->tag() << item;
  for (auto c : item->childItems()) {
    printDetails(c, indent + "  ");
  }
}

void ReactView::updatePolish()
{
  // XXX:
  ReactFlexLayout::get(this)->setWidth(this->width());
  ReactFlexLayout::get(this)->setHeight(this->height());
  ReactItem::updatePolish();

  //
  //  qDebug() << "Items after polish";
  // printDetails(this, "  ");
}

bool ReactView::childMouseEventFilter(QQuickItem* item, QEvent* e)
{
  qDebug() << __PRETTY_FUNCTION__;
}
