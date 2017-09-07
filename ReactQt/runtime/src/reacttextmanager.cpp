
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
  return new ReactPropertyHandler(object);
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
