
#include "reactevents.h"

QString normalizeInputEventName(const QString& eventName)
{
  QString tmp = eventName;
  if (eventName.startsWith("top"))
    return tmp;

  if (eventName.startsWith("on")) {
    tmp.replace(0, 2, "top");
  } else {
    tmp[0] = tmp[0].toUpper();
    tmp = "top" + tmp;
  }

  return tmp;
}
