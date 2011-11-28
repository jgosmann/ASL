#include "shaderlistview.h"

ShaderListView::ShaderListView(QWidget *parent) :
    QListView(parent) ,
    currentID(0)
{
    currentRow = -1;
    init();

}

ShaderListView::~ShaderListView(){
    shaderMap.clear();
    if(itemModel)
        delete itemModel;
}

void ShaderListView::init(){

    shaderMap = QMap<int,QSharedPointer<Shader> >();
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
    connect(this,SIGNAL(indexesMoved(const QModelIndexList &)), this, SIGNAL(renderShaderList(void)));


}

void ShaderListView::addShader(QSharedPointer<Shader> shader){
    QStandardItem* item = new QStandardItem(shader->name());
    item->setCheckable(true);
    item->setData(QVariant(currentID));
    shaderMap.insert(currentID,shader);
    itemModel->appendRow(item);

    ++currentID;
}

void ShaderListView::removeSelectedShader(){
    if(currentRow >= 0 && currentRow < itemModel->rowCount()){
        int key = itemModel->item(currentRow)->data().value<int>();
        shaderMap.remove(key);
        itemModel->removeRow(currentRow);
        emit renderShaderList();
    }
}


void ShaderListView::clearList(){
    itemModel->clear();
    shaderMap.clear();
}

void ShaderListView::clickedOnShader(const QModelIndex &index){
    QStandardItem* item = itemModel->itemFromIndex(index);
    currentRow = index.row();

    int key = item->data().value<int>();
    emit shaderClicked( shaderMap.keys().indexOf(key) );
    emit renderShaderList();

}

QList<QSharedPointer<Shader> > ShaderListView::getCheckedShaders(){
    QList<QSharedPointer<Shader> > list;
    for(int i = 0; i < itemModel->rowCount();i++){
        QStandardItem* item = itemModel->item(i);
        if(item->checkState()){
            int key = item->data().value<int>();
            list.append(shaderMap.value(key));
        }
    }
    return list;
}

QList<QSharedPointer<Shader> > ShaderListView::getAllShaders(){
    QList<QSharedPointer<Shader> > list;
    for(int i = 0; i < itemModel->rowCount();i++){
        QStandardItem* item = itemModel->item(i);
        int key = item->data().value<int>();
           list.append(shaderMap.value(key));
    }

    return list;
}

