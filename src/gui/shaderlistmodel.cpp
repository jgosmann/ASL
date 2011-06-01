#include "shaderlistmodel.h"

ShaderListModel::ShaderListModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

int ShaderListModel::rowCount(const QModelIndex &parent) const{
    return shaderList.size();
}

QVariant ShaderListModel::data(const QModelIndex &index, int role) const{
    return QVariant(shaderList.at(index.row())->name());
}

void ShaderListModel::addShader(QSharedPointer<Shader> shader){
    beginInsertRows(QModelIndex(),shaderList.size()-1,shaderList.size());
    shaderList.append(shader);
    endInsertRows();
//    QStandardItem* item = new QStandardItem(shader->name());
//    item->setCheckable(true);

}
