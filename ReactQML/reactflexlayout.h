#ifndef REACTFLEXPROPERTIES_H
#define REACTFLEXPROPERTIES_H

#include <QObject>
#include <QtQml>
#include <QScopedPointer>


// XXX: The enumeration values are in sync with those defined in the layout code

class ReactFlexLayoutPrivate;
class ReactFlexLayout : public QObject
{
  Q_OBJECT
  Q_PROPERTY(double flex READ flex WRITE setFlex NOTIFY flexChanged)
  Q_PROPERTY(Direction direction READ direction WRITE setDirection NOTIFY directionChanged)
  Q_PROPERTY(Justify justify READ justify WRITE setJustify NOTIFY justifyChanged)
  Q_PROPERTY(Alignment selfAlignment READ selfAlignment WRITE setSelfAlignment NOTIFY selfAlignmentChanged)
  Q_PROPERTY(Alignment itemAlignment READ itemAlignment WRITE setItemAlignment NOTIFY itemAlignmentChanged)
  Q_PROPERTY(Position position READ position WRITE setPosition NOTIFY positionChanged)
  Q_PROPERTY(Wrap wrap READ wrap WRITE setWrap NOTIFY wrapChanged)
  Q_PROPERTY(double top READ top WRITE setTop NOTIFY topChanged)
  Q_PROPERTY(double right READ right WRITE setRight NOTIFY rightChanged)
  Q_PROPERTY(double bottom READ bottom WRITE setBottom NOTIFY bottomChanged)
  Q_PROPERTY(double left READ left WRITE setLeft NOTIFY leftChanged)
  Q_PROPERTY(double width READ width WRITE setWidth NOTIFY widthChanged)
  Q_PROPERTY(double height READ height WRITE setHeight NOTIFY heightChanged)

  Q_ENUMS(Direction)
  Q_ENUMS(Justify)
  Q_ENUMS(Alignment)
  Q_ENUMS(Position)
  Q_ENUMS(Wrap)

  Q_DECLARE_PRIVATE(ReactFlexLayout)

public:
  ReactFlexLayout(QObject* parent = 0);
  ~ReactFlexLayout();

  double flex() const;
  void setFlex(double flex);

  enum Direction { DirectionColumn = 0, DirectionColumnReverse, DirectionRow, DirecctionRowReverse };
  Direction direction() const;
  void setDirection(Direction direction);

  enum Justify { JustifyFlexStart = 0, JustifyCenter, JustifyFlexEnd, JustifySpaceBetween, JustifySpaceAround };
  Justify justify() const;
  void setJustify(Justify justify);

  enum Alignment { AlignementAuto = 0, AlignmentFlexStart, AlignmentCenter, AlignmentFlexEnd, AlignmentStretch };
  Alignment selfAlignment() const;
  void setSelfAlignment(Alignment alignment);

  Alignment itemAlignment() const;
  void setItemAlignment(Alignment alignment);

  enum Position { PositionRelative = 0, PositionAbsolute };
  Position position() const;
  void setPosition(Position position);

  enum Wrap { WrapNo = 0, WrapYes };
  Wrap wrap() const;
  void setWrap(Wrap wrap);

  double top() const;
  void setTop(double top);

  double right() const;
  void setRight(double right);

  double bottom() const;
  void setBottom(double bottom);

  double left() const;
  void setLeft(double left);

  double width() const;
  void setWidth(double width);

  double height() const;
  void setHeight(double height);

  void layout();

  static ReactFlexLayout* get(QQuickItem* item, bool create = true);
  static ReactFlexLayout* qmlAttachedProperties(QObject* object);

Q_SIGNALS:
  void flexChanged();
  void directionChanged();
  void justifyChanged();
  void selfAlignmentChanged();
  void itemAlignmentChanged();
  void positionChanged();
  void wrapChanged();
  void topChanged();
  void rightChanged();
  void bottomChanged();
  void leftChanged();
  void widthChanged();
  void heightChanged();

private:
  QScopedPointer<ReactFlexLayoutPrivate> d_ptr;
};

QML_DECLARE_TYPEINFO(ReactFlexLayout, QML_HAS_ATTACHED_PROPERTIES)

#endif // REACTFLEXPROPERTIES_H
