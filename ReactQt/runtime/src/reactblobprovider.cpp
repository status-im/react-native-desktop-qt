/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "reactblobprovider.h"
#include "reactbridge.h"

class ReactBlobProviderPrivate {
public:

  ReactBridge* bridge = nullptr;
};

ReactBlobProvider::ReactBlobProvider(QObject* parent)
  : QObject(parent)
  , d_ptr(new ReactBlobProviderPrivate)
{
}

ReactBlobProvider::~ReactBlobProvider()
{
}

void ReactBlobProvider::setBridge(ReactBridge* bridge)
{
  Q_D(ReactBlobProvider);
  d->bridge = bridge;
}

QString ReactBlobProvider::moduleName()
{
  return "RCTBlobModule";
}

QList<ReactModuleMethod*> ReactBlobProvider::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap ReactBlobProvider::constantsToExport()
{
  return QVariantMap{};
}
