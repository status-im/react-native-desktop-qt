
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "jscutilities.h"

#include <QJsonDocument>

namespace utilities {

folly::dynamic qvariantToDynamic(const QVariant& value) {
    QJsonDocument doc = QJsonDocument::fromVariant(value);
    return folly::parseJson(doc.toJson(QJsonDocument::Compact).toStdString());
}

QVariant dynamicToQVariant(const folly::dynamic& value) {
    std::string jsonStr = folly::toJson(value);
    return QJsonDocument::fromJson(jsonStr.c_str()).toVariant();
}

} // namespace utilities
