#include "scrollviewmodel.h"
#include <QDebug>

ScrollViewModel::ScrollViewModel(QQmlEngine* engine, QObject* parent) : QAbstractListModel(parent) {
    m_engine = engine;
}

int ScrollViewModel::rowCount(const QModelIndex& parent = QModelIndex()) const {
    return count();
}

int ScrollViewModel::count() const {
    return m_model.size();
}

QVariant ScrollViewModel::data(const QModelIndex& index, int role = Qt::DisplayRole) const {
    Q_UNUSED(role)
    if (!hasIndex(index.row(), index.column(), index.parent()))
        return {};

    return m_model[index.row()];
}

QQuickItem* ScrollViewModel::allocateNewItem() {
    QUrl fileUrl("qrc:/ScrollViewComponent.qml");

    QQmlComponent component(m_engine);
    component.loadUrl(fileUrl);
    if (!component.isReady()) {
        qCritical() << QString("Component for %1 is not loaded").arg(fileUrl.toString());
    }

    QObject* createdObject = component.create();
    if (createdObject == nullptr) {
        qCritical() << QString("Unable to construct item from component %1").arg(fileUrl.toString());
    }
    return qobject_cast<QQuickItem*>(createdObject);
}

void ScrollViewModel::insert(QQuickItem* item, int position) {
    beginInsertRows(QModelIndex(), position, position);
    m_model.insert(position, QVariant::fromValue(item));
    endInsertRows();
}

QVariant ScrollViewModel::takeAt(int position) {
    beginRemoveRows(QModelIndex(), position, position);
    QVariant result = m_model.takeAt(position);
    endRemoveRows();
    return result;
}
