#ifndef SHADERITEM_H
#define SHADERITEM_H

#include <QStandardItem>
#include <QSharedPointer>
#include <asl/annotatedglshaderprogram.h>

typedef class asl::AnnotatedGLShaderProgram Shader;

class ShaderItem : public QStandardItem
{
public:

    explicit ShaderItem( QSharedPointer<Shader> shader);

    QSharedPointer<Shader> getShader();

    QStandardItem * clone () const;

    int type() const{
        return 1223;
    }

private:
    QSharedPointer<Shader> shader;


};

#endif // SHADERITEM_H
