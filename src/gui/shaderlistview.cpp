#include "shaderlistview.h"

ShaderListView::ShaderListView(QWidget *parent) :
    QListView(parent)
{


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





}

void ShaderListView::addShader(QSharedPointer<Shader> shader){
//    shaderList.append(shader);
//    QStandardItem* item = new QStandardItem(shader->name());
    QStandardItem* item = new ShaderItem(shader);
    item->setCheckable(true);

    itemModel->appendRow(item);
//    itemModel->addShader(shader);
}


void ShaderListView::clearList(){
    itemModel->clear();
}
