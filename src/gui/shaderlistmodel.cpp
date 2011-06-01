#include "shaderlistmodel.h"

ShaderListModel::ShaderListModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

int ShaderListModel::rowCount(const QModelIndex &parent) const{
    return shaderList.size();
}

QVariant ShaderListModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= shaderList.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return QVariant(shaderList.at(index.row())->name());
    else
        return QVariant();

}

QVariant ShaderListModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QString("Column %1").arg(section);
    else
        return QString("Row %1").arg(section);
}

void ShaderListModel::addShader(QSharedPointer<Shader> shader){
    QModelIndex index = createIndex(shaderList.size(),0,shaderList.size());
    beginInsertRows(QModelIndex(),shaderList.size()-1,shaderList.size());
    shaderList.append(shader);
    emit dataChanged(index, index);
    endInsertRows();
//    QStandardItem* item = new QStandardItem(shader->name());
//    item->setCheckable(true);

}
