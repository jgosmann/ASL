#include "shaderitem.h"


ShaderItem::ShaderItem(QSharedPointer<Shader> pShader):
        QStandardItem(pShader->name())
{
    shader = pShader;
    this->setData(QVariant::fromValue(pShader));
//    this->setText(pShader->name());
}

QSharedPointer<Shader> ShaderItem::getShader(){
//    return shader;
    QSharedPointer<Shader> s = data().value<QSharedPointer<Shader> >();
    assert(s != NULL);
    return s;
}

QStandardItem * ShaderItem::clone () const{

    ShaderItem* cl = new ShaderItem(shader);
    return (QStandardItem*) cl;
}


