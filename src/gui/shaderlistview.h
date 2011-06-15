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

#include <iostream>


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
    /**
      * returns a list of all checked shaders
      * the order of the list matches with the view
      */
    QList<QSharedPointer<Shader> > getCheckedShaders();
    /**
      * returns all shaders
      * the order of the list matches with the view
      */
    QList<QSharedPointer<Shader> > getAllShaders();





signals:
    void shaderClicked(QSharedPointer<Shader> shader);

public slots:

private slots:
    void clickedOnShader(const QModelIndex & index);

};

#endif // SHADERLISTVIEW_H
