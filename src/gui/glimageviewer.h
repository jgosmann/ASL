#ifndef GLIMAGEVIEWER_H
#define GLIMAGEVIEWER_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
//#include <QRectF>
//#include <QKeyEvent>
//#include <QWheelEvent>

#include "mainwindow.h"

namespace gui {

class GLImageViewer : public QGraphicsView
{
    Q_OBJECT

    public:
        GLImageViewer(QWidget *parent = NULL);
        virtual ~GLImageViewer();

    public slots:
        void setImage(const QImage &img);
        void setImageZoom(int value);

    signals:
        void zoomChanged(int value);

    protected:
        //void virtual keyPressEvent(QKeyEvent *event);
        //void virtual wheelEvent(QWheelEvent *event);

    private:
        QGraphicsScene m_scene;

};

}

#endif // GLIMAGEVIEWER_H
