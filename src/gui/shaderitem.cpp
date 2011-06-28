#include "shaderitem.h"


ShaderItem::ShaderItem(QSharedPointer<Shader> pShader):
        QStandardItem(pShader->name())
{
    shader = pShader;
//    this->setText(pShader->name());
}

QSharedPointer<Shader> ShaderItem::getShader(){
    return shader;
}

QStandardItem * ShaderItem::clone () const{

    ShaderItem* cl = new ShaderItem(shader);
    return (QStandardItem*) cl;
}


