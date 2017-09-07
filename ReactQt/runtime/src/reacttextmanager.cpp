
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

#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlProperty>

#include <QDebug>

#include "reacttextmanager.h"
#include "reactbridge.h"
#include "reactflexlayout.h"
#include "reactpropertyhandler.h"


ReactTextManager::ReactTextManager(QObject* parent)
  : ReactRawTextManager(parent)
{
}

ReactTextManager::~ReactTextManager()
{
}

ReactViewManager* ReactTextManager::viewManager()
{
  return this;
}

ReactPropertyHandler* ReactTextManager::propertyHandler(QObject* object)
{
  Q_ASSERT(qobject_cast<QQuickItem*>(object) != nullptr);

  //we keep track of all assigned properties because if property not assigned explicitly,
  //it should be taken from parent
  return new ReactPropertyHandler(object, [&](QObject* object, QMetaProperty property, const QVariant& value) {
    m_explicitlySetProps[object].insert(property.name());
  });
}

QString ReactTextManager::moduleName()
{
  return "RCTTextManager";
}

QList<ReactModuleMethod*> ReactTextManager::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap ReactTextManager::constantsToExport()
{
  return QVariantMap{};
}

bool ReactTextManager::shouldLayout() const
{
  return true;
}

QString ReactTextManager::qmlComponentFile() const
{
  return ":/qml/ReactText.qml";
}

void ReactTextManager::configureView(QQuickItem* view) const
{
  ReactRawTextManager::configureView(view);
  view->setProperty("textManager", QVariant::fromValue((QObject*)this));
}


void ReactTextManager::hookLayout(QQuickItem* textItem)
{
  ReactFlexLayout* fl = ReactFlexLayout::get(textItem);
  fl->setMeasureFunction([=](double width, FlexMeasureMode widthMode, double height, FlexMeasureMode heightMode) {

    resizeToWidth(textItem, width);

    double w = textItem->width();
    double ch = textItem->property("contentHeight").toDouble();
    return std::make_pair(w, ch);
  });
}

void ReactTextManager::resizeToWidth(QQuickItem* textItem, double width)
{
  double contentWidth = textItem->property("contentWidth").value<double>();
  double sw = 0;
  if (std::isnan(width))
  {
    sw = contentWidth;
  }
  else
  {
    sw = contentWidth == 0 ? width : qMin(contentWidth, width);
  }
  textItem->setWidth(sw);
}

QVariant ReactTextManager::nestedPropertyValue(QQuickItem* item, const QString& propertyName)
{
  //return value if it is explicitly set on current item
  if(propertyExplicitlySet(item, propertyName))
  {
    return item->property(propertyName.toStdString().c_str());
  }

  //check if any of text parents has this property explicitly set
  QQuickItem* parent = parentTextItem(item);
  while(parent)
  {
    if(propertyExplicitlySet(parent, propertyName))
    {
      return parent->property(propertyName.toStdString().c_str());
    }
    parent = parentTextItem(parent);
  }

  //no parents has this property explicitly set, so we use default value
  return item->property(propertyName.toStdString().c_str());
}

QQuickItem* ReactTextManager::parentTextItem(QQuickItem* textItem)
{
  if(!textItem)
    return nullptr;

  auto visualParent = textItem->parentItem();
  if(!visualParent)
    return nullptr;

  QVariant typeName = visualParent->property("typeName");
  if(!typeName.isValid())
    return nullptr;

  return (typeName.toString() == "ReactText") ? static_cast<QQuickItem*>(visualParent) : nullptr;

}

bool ReactTextManager::propertyExplicitlySet(QQuickItem* item, const QString& propertyName)
{
  if(m_explicitlySetProps.contains(item))
  {
    return m_explicitlySetProps[item].contains(propertyName);
  }
  return false;
}
