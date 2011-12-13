#ifndef SHADERLISTVIEW_H
#define SHADERLISTVIEW_H

#include <QListView>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>
#include <QSharedPointer>
#include <QDragEnterEvent>

#include <asl/annotatedglshaderprogram.h>
#include <gui/shaderitemlist.h>
#include <gui/shaderitem.h>

#include <iostream>

// !! abbreviation
typedef class asl::AnnotatedGLShaderProgram Shader;

class ShaderListView : public QListView
{
    Q_OBJECT
public:
    explicit ShaderListView(QWidget *parent = 0);

    ~ShaderListView();
    void init();


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


protected:
    bool eventFilter(QObject *watched, QEvent *event);


signals:
    void shaderClicked(int index);
    void renderShaderList();

public slots:
    void removeSelectedShader();

private slots:
    void clickedOnShader(const QModelIndex & index);

private:
    ShaderItemList *itemModel;
    int currentID;
    int currentRow;
    QMap<int,QSharedPointer<Shader> > shaderMap;

};



#endif // SHADERLISTVIEW_H
