
#include <QString>
#include <QVariant>
#include <QQuickItem>
#include <QQmlProperty>

#include "reactviewmanager.h"
#include "reactitem.h"
#include "reactbridge.h"
#include "reactflexlayout.h"

namespace {
static QMap<QString, ReactFlexLayout::Direction> directions{
  { "row", ReactFlexLayout::DirectionRow },
  { "column", ReactFlexLayout::DirectionColumn }
};
static QMap<QString, ReactFlexLayout::Alignment> alignments{
  { "flex-start", ReactFlexLayout::AlignmentFlexStart },
  { "flex-end", ReactFlexLayout::AlignmentFlexEnd },
  { "center", ReactFlexLayout::AlignmentCenter },
  { "stretch", ReactFlexLayout::AlignmentStretch }
};
static QMap<QString, ReactFlexLayout::Justify> justifys{
  { "flex-start", ReactFlexLayout::JustifyFlexStart },
  { "flex-end", ReactFlexLayout::JustifyFlexEnd },
  { "center", ReactFlexLayout::JustifyCenter },
  { "space-between", ReactFlexLayout::JustifySpaceBetween },
  { "space-around", ReactFlexLayout::JustifySpaceAround }
};
static QMap<QString, ReactFlexLayout::Position> positions{
  { "absolute", ReactFlexLayout::PositionAbsolute },
  { "relative", ReactFlexLayout::PositionRelative }
};
static QMap<QString, ReactFlexLayout::Wrap> wraps{
  { "wrap", ReactFlexLayout::WrapYes },
  { "nowrap", ReactFlexLayout::WrapNo }
};

void applyFlexProperties(QQuickItem* item, const QVariantMap& properties)
{
  qDebug() << __PRETTY_FUNCTION__ << item << properties;
  ReactFlexLayout* flex = ReactFlexLayout::get(item);

  for (const QString& key : properties.keys()) {
    if (key == "flex") {
      flex->setFlex(properties.value(key).toDouble());
    } else if (key == "flexDirection") {
      flex->setDirection(directions[properties.value(key).toString()]);
    } else if (key == "alignItems") {
      flex->setItemAlignment(alignments[properties.value(key).toString()]);
    } else if (key == "justifyContent") {
      flex->setJustify(justifys[properties.value(key).toString()]);
    } else if (key == "position") {
      flex->setPosition(positions[properties.value(key).toString()]);
    } else if (key == "flexWrap") {
      flex->setWrap(wraps[properties.value(key).toString()]);
    } else if (key == "top") {
      flex->setTop(properties.value(key).toDouble());
    } else if (key == "right") {
      flex->setRight(properties.value(key).toDouble());
    } else if (key == "bottom") {
      flex->setBottom(properties.value(key).toDouble());
    } else if (key == "left") {
      flex->setLeft(properties.value(key).toDouble());
    } else if (key == "width") {
      flex->setWidth(properties.value(key).toDouble());
    } else if (key == "height") {
      flex->setHeight(properties.value(key).toDouble());
    }
  }
}
}


ReactViewManager::ReactViewManager(QObject *parent)
  : QObject(parent)
{
}

ReactViewManager::~ReactViewManager()
{
}

void ReactViewManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

// TODO: this doesnt seem right
ReactViewManager* ReactViewManager::viewManager()
{
  return this;
}

QString ReactViewManager::moduleName()
{
  return "RCTViewManager";
}

QStringList ReactViewManager::methodsToExport()
{
  return QStringList{};
}

QVariantMap ReactViewManager::constantsToExport()
{
  return QVariantMap{};
}

QStringList ReactViewManager::customDirectEventTypes()
{
  return QStringList{};
}

QStringList ReactViewManager::customBubblingEventTypes()
{
  return QStringList{"press", "change", "focus", "blur", "submitEditing", "endEditing", "touchStart", "touchMove", "touchCancel", "touchEnd"};
}

QQuickItem* ReactViewManager::view(const QVariantMap& properties) const
{
  qDebug() << __PRETTY_FUNCTION__ << "properties" << properties;

  QQmlComponent component(m_bridge->qmlEngine());
  component.setData("import QtQuick 2.4\nRectangle{}", QUrl());
  if (!component.isReady())
    qCritical() << "Rectangle not ready!";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to construct Rectangle";
    return nullptr;
  }

  applyProperties(item, properties);

  return item;
}

void ReactViewManager::applyProperties(QQuickItem* item, const QVariantMap& properties) const
{
  qDebug() << __PRETTY_FUNCTION__ << item << properties;
  if (properties.isEmpty())
    return;

  // XXX: maybe should just use qt private stuff

  // Apply Flex properties
  applyFlexProperties(item, properties);

  // Set properties
  // XXX: hmm, translation rules, etc
  Q_FOREACH(const QString& key, properties.keys()) {
    if (key == "backgroundColor") {
      item->setProperty("color", QColor(properties.value(key).toUInt()));
    } else if (key == "borderRadius") {
      item->setProperty("radius", properties.value(key));
    } else if (key == "opacity") {
      item->setOpacity(properties.value(key).toDouble());
    }
  }
}
