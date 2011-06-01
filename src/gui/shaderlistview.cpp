#include "shaderlistview.h"

ShaderListView::ShaderListView(QWidget *parent) :
    QListView(parent)
{


}

void ShaderListView::init(){

    itemModel = new ShaderListModel();
    this->setModel(itemModel);


}

void ShaderListView::addShader(QSharedPointer<Shader> shader){
//    shaderList.append(shader);
//    QStandardItem* item = new QStandardItem(shader->name());
//    item->setCheckable(true);

//    itemModel->appendRow(item);
    itemModel->addShader(shader);
}

void ShaderListView::moveSelShaderDown(int shaderPos){
//    QStandardItem* tmp = itemModel->item(shaderPos);
//    itemModel->removeRow(shaderPos);
//    itemModel->insertRow(shaderPos+1,tmp);
}
