#ifndef SHADERLISTVIEW_H
#define SHADERLISTVIEW_H

#include <QListView>
#include <QStringListModel>

class ShaderListView : public QListView
{
    Q_OBJECT
public:
    explicit ShaderListView(QWidget *parent = 0);
    void init();
    QStringListModel* model;

signals:

public slots:

};

#endif // SHADERLISTVIEW_H
