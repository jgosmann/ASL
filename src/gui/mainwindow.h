#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>

#include <QMainWindow>
#include <QCoreApplication>
#include <QScrollArea>
#include <QGLContext>
#include <QGLFormat>
#include <QSharedPointer>
#include <QMessageBox>
#include <QLabel>

#include <asl/annotatedglshaderprogram.h>
#include <asl/defaultaslprogramcompiler.h>
#include <asl/gltypeinfo.h>

#include "glimagerenderer.h"
#include "shaderparametercontrolfactory.h"
#include "shaderparametercontrolhandle.h"
#include "shaderparameterbundle.h"

namespace Ui {
    class MainWindow;
}

namespace gui
{
  typedef class asl::AnnotatedGLShaderProgram Shader;

  class MainWindow : public QMainWindow, public UniformSetter
  {
      Q_OBJECT

  public:
      explicit MainWindow(QWidget *parent = 0);
      ~MainWindow();

      void setUniforms( unsigned int index );

  public slots:
      void loadShaderDialog();
      void loadImageFile();
      void saveImage();
      void showControls( int index );

  signals:
      void exitProgram();

  private:
      Ui::MainWindow *ui;

      QGLContext *m_sharedContext;
      // this class connects the shaderList with the glImageViewer-widget
      GLImageRenderer *m_glImageRenderer;

      asl::DefaultASLProgramCompiler compiler;

      ShaderParameterBundle m_shaderParameterBundle;

  private slots:
      void emitExit();
  };
}

#endif // MAINWINDOW_H
