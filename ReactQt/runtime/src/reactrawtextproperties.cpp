
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


class ReactRawTextPropertiesPrivate
{
public:
  bool dirty;
  QQuickItem* item;
  QString text;

  void setDirty(bool dirty) {
    this->dirty = true;
    ReactTextProperties* tp = ReactTextProperties::get(item->parentItem(), false);
    if (tp != nullptr) {
      tp->setDirty(dirty);
    }
  }

  static ReactRawTextPropertiesPrivate* get(ReactRawTextProperties* rtp) {
    return rtp->d_func();
  }
};


ReactRawTextProperties::ReactRawTextProperties(QObject* parent)
  : ReactPropertyHandler(parent)
  , d_ptr(new ReactRawTextPropertiesPrivate)
{
  Q_D(ReactRawTextProperties);
  d->dirty = false;
  d->item = qobject_cast<QQuickItem*>(parent);
  if (d->item == nullptr) {
    qCritical() << "ReactRawTextProperties only applies to visual items";
  }
}

ReactRawTextProperties::~ReactRawTextProperties()
{
}

QString ReactRawTextProperties::text() const
{
  return d_func()->text;
}

void ReactRawTextProperties::setText(const QString& text)
{
  Q_D(ReactRawTextProperties);
  if (d->text == text)
    return;
  d->text = text;
  d->setDirty(true);
}


ReactRawTextProperties* ReactRawTextProperties::get(QQuickItem* item, bool create)
{
  return qobject_cast<ReactRawTextProperties*>(qmlAttachedPropertiesObject<ReactRawTextProperties>(item, create));
}

ReactRawTextProperties* ReactRawTextProperties::qmlAttachedProperties(QObject* object)
{
  return new ReactRawTextProperties(object);
}
