#ifndef REACTITEM_H
#define REACTITEM_H

#include <QQuickItem>
#include <QScopedPointer>


class ReactItemPrivate;
class ReactItem : public QQuickItem
{
  Q_OBJECT

public:
  ReactItem(QQuickItem *parent = 0);
  ~ReactItem();

protected:
  void updatePolish();

private:
  QScopedPointer<ReactItemPrivate> d_ptr;
};

#endif // REACTITEM_H
