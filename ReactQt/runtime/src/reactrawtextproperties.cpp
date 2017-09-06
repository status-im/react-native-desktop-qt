
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

#include <QQuickItem>

#include "reacttextproperties.h"
#include "reactrawtextproperties.h"


ReactRawTextProperties::ReactRawTextProperties(QObject* parent)
  : ReactPropertyHandler(parent)
{
  dirty = false;
  item = qobject_cast<QQuickItem*>(parent);
  if (item == nullptr) {
    qCritical() << "ReactRawTextProperties only applies to visual items";
  }
}

ReactRawTextProperties::~ReactRawTextProperties()
{
}

void ReactRawTextProperties::setDirty(bool dirty)
{
  this->dirty = true;
  ReactTextProperties* tp = ReactTextProperties::get(item->parentItem(), false);
  if (tp != nullptr) {
    tp->setDirty(dirty);
  }
}

QString ReactRawTextProperties::text() const
{
  return textString;
}

void ReactRawTextProperties::setText(const QString& text)
{
  if (textString == text)
    return;
  textString = text;
  setDirty(true);
}


ReactRawTextProperties* ReactRawTextProperties::get(QQuickItem* item, bool create)
{
  return qobject_cast<ReactRawTextProperties*>(qmlAttachedPropertiesObject<ReactRawTextProperties>(item, create));
}

ReactRawTextProperties* ReactRawTextProperties::qmlAttachedProperties(QObject* object)
{
  return new ReactRawTextProperties(object);
}
