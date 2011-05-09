#include "shaderlistview.h"

ShaderListView::ShaderListView(QWidget *parent) :
    QListView(parent)
{


}

void ShaderListView::init(){
    QStringList list;
    list.append("erstens");
    list.append("zweitens");
    list.append("drittens");
    model = new QStringListModel(list);
    this->setModel(model);

    model->insertRow(2);


}
