#include "shaderlistview.h"

ShaderListView::ShaderListView(QWidget *parent) :
    QListView(parent)
{
    currentRow = -1;
    init();

}

void ShaderListView::init(){

    //Initializes the model
    itemModel = new ShaderItemList();
    this->setModel(itemModel);

    //All settings for drag and drop
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setDragEnabled(true);
    this->setDragDropMode(QAbstractItemView::InternalMove);
    this->viewport()->setAcceptDrops(true);
    this->setDropIndicatorShown(true);

    this->setDragDropOverwriteMode(false);

    connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(clickedOnShader(QModelIndex)));


}

void ShaderListView::addShader(QSharedPointer<Shader> shader){
    QStandardItem* item = new ShaderItem(shader);
    item->setCheckable(true);
    itemModel->appendRow(item);
}

void ShaderListView::removeSelectedShader(){
    if(currentRow >= 0 && currentRow < itemModel->rowCount()){
       itemModel->removeRow(currentRow);
    }
}


void ShaderListView::clearList(){
    itemModel->clear();
}

void ShaderListView::clickedOnShader(const QModelIndex &index){
    ShaderItem* item = (ShaderItem*)(itemModel->itemFromIndex(index));
    currentRow = index.row();
    emit shaderClicked(item->getShader());

}

QList<QSharedPointer<Shader> > ShaderListView::getCheckedShaders(){
    QList<QSharedPointer<Shader> > list;
    for(int i = 0; i < itemModel->rowCount();i++){
        ShaderItem* item = (ShaderItem*)(itemModel->item(i));
        if(item->checkState()){
           list.append(item->getShader());
        }
    }
    return list;
}

QList<QSharedPointer<Shader> > ShaderListView::getAllShaders(){
    QList<QSharedPointer<Shader> > list;
    for(int i = 0; i < itemModel->rowCount();i++){
        ShaderItem* item = (ShaderItem*)(itemModel->item(i));
           list.append(item->getShader());
    }

    return list;
}
