
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

#include <QtQml>
#include <QMetaType>

#include "reactplugin.h"
#include "reactattachedproperties.h"
#include "reactflexlayout.h"
#include "reacttextproperties.h"
#include "reactrawtextproperties.h"
#include "reactimagemanager.h"
#include "reactviewmanager.h"
#include "reactitem.h"
#include "reactview.h"


void ReactPlugin::registerTypes(const char* uri)
{
  qmlRegisterUncreatableType<ReactAttachedProperties>("React", 0, 1, "React", "React is not meant to be created directly");
  qmlRegisterUncreatableType<ReactFlexLayout>("React", 0, 1, "Flex", "Flex is not meant to be created directly");
  qmlRegisterUncreatableType<ReactTextProperties>("React", 0, 1, "Text", "Text is not meant to be created directly");
  qmlRegisterUncreatableType<ReactRawTextProperties>("React", 0, 1, "RawText", "Text is not meant to be created directly");
  qmlRegisterType<ReactItem>("React", 0, 1, "Item");
  qmlRegisterType<ReactView>("React", 0, 1, "RootView");
  qmlRegisterUncreatableType<ReactImageManager>("React", 0, 1, "ReactImageManager", "ReactImageManager is not meant to be created directly");
  qmlRegisterUncreatableType<ReactViewManager>("React", 0, 1, "ReactViewManager", "ReactViewManager is not meant to be created directly");

}
