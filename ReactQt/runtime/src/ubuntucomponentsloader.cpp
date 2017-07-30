
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

#include <QPair>
#include <QString>
#include <QList>
#include <QCoreApplication>
#include <QLibraryInfo>
#include <QFile>

#include <QDebug>

#include "ubuntucomponentmodule.h"
#include "ubuntucomponentsloader.h"



namespace
{
const char* components_path = "/Ubuntu/Components/";

QStringList blackList{
  "OptionSelectorDelegate", "Header", "UbuntuNumberAnimation", "Scrollbar",
  "Page", "TextField"
};

struct UbuntuComponent {
  QString name;
  QString version;
  QString path;
};

QList<UbuntuComponent> findComponents(QIODevice *src)
{
  Q_ASSERT(!src->isSequential());

  QMap<QString, UbuntuComponent> cm;

  // XXX: this is a wee bit rough, but will do for now
  forever {
    QString line = QString(src->readLine(255).simplified()); // XXX: constant
    if (line.isEmpty()) {
      if (src->atEnd())
        break;
      continue;
    }

    if (line.startsWith("module") || line.startsWith("plugin") ||
        line.startsWith("internal") || line.startsWith("#version") ||
        line.startsWith("singleton"))
      continue;

    QStringList parts = line.split(" ");
    if (parts.size() != 3)
      continue;

    const QString& name = parts[0];
    const QString& version = parts[1];
    const QString& path = parts[2];

    if (!path.endsWith("qml"))
      continue;

    if (blackList.contains(name))
      continue;

    if (cm.contains(name)) {
      if (cm[name].version.toDouble() > version.toDouble())
        continue;
    }

    cm.insert(name, UbuntuComponent{name, version, path});
  }

  return cm.values();
}
}

QObjectList UbuntuComponentsLoader::availableModules()
{
  QObjectList modules;

  QString basePath = QLibraryInfo::location(QLibraryInfo::Qml2ImportsPath) + components_path;
  QString qmlDirFilename = basePath + "qmldir";
  QFile qmlFile(qmlDirFilename);
  if (!qmlFile.open(QIODevice::ReadOnly)) {
    qWarning() << "Unable to locate Ubuntu Components qmldir!";
    return modules;
  }

  for (const UbuntuComponent& e : findComponents(&qmlFile)) {
    modules << new UbuntuComponentModule(e.name, e.version, basePath + e.path);
  }

  return modules;
}

