#ifndef SHADERLISTVIEW_H
#define SHADERLISTVIEW_H

#include <QListView>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>
#include <QSharedPointer>

#include <asl/annotatedglshaderprogram.h>
#include <gui/shaderlistmodel.h>
#include <gui/shaderitemlist.h>
#include <gui/shaderitem.h>


typedef class asl::AnnotatedGLShaderProgram Shader;

class ShaderListView : public QListView
{
    Q_OBJECT
public:
    explicit ShaderListView(QWidget *parent = 0);
    void init();
//    ShaderListModel *itemModel;
    ShaderItemList *itemModel;
    QList< QSharedPointer<Shader> > shaderList;

    /**
      * Clears the current shader List and replaces it with the given pShaderList
      */
    void setShaderList(QList< QSharedPointer<Shader> > pShaderList);
    /**
      * Adds a shader to the List
      */
    void addShader(QSharedPointer<Shader> shader);
    /**
      * Clears the List
      */
    void clearList();



signals:

public slots:

};

#endif // SHADERLISTVIEW_H
