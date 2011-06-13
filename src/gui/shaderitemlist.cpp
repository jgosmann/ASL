#include "shaderitemlist.h"

ShaderItemList::ShaderItemList(QObject *parent) :
    QStandardItemModel(parent)
{
}

Qt::ItemFlags ShaderItemList::flags ( const QModelIndex & index ) const{
    if(index.isValid()){
        return (Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
    } else {
    return Qt::ItemIsDropEnabled;
    }
}
