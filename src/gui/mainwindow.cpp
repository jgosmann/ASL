#include "mainwindow.h"
#include "ui_mainwindow.h"


using namespace gui;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // set working directory to execution directory
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    ui->setupUi(this);

    m_glImageRenderer = new GLImageRenderer(*this, this);

    connect(ui->spinBox_Zoom, SIGNAL(valueChanged(int)), 
            ui->glDisplay, SLOT(setImageZoom(int)));
    connect(ui->glDisplay, SIGNAL(zoomChanged(int)), 
            ui->spinBox_Zoom, SLOT(setValue(int)));

    connect(ui->action_LoadImage, SIGNAL(triggered()),
            this, SLOT(loadImageFile()));
    connect(ui->action_SaveImage, SIGNAL(triggered()),
            this, SLOT(saveImage()));
    connect(ui->checkBox_ApplyShaders, SIGNAL(stateChanged(int)), 
            m_glImageRenderer, SLOT(enableShaders(int)));

    connect(m_glImageRenderer, SIGNAL(updated(const QImage &)),
            ui->glDisplay, SLOT(setImage(const QImage &)));

    connect(ui->pushButton_AddShader,SIGNAL(clicked()),
            this,SLOT(loadShaderDialog()));
    connect(ui->pushButton_RemoveShader,SIGNAL(clicked()),
            this, SLOT(removeShader()));

    connect(ui->listView_ShaderList, SIGNAL(renderShaderList(void)),
            this, SLOT(render(void)));

    connect(ui->listView_ShaderList, SIGNAL(
              shaderClicked(int)),
            this, SLOT(
              showControls(int)));

    connect(ui->action_Exit,SIGNAL(triggered()),this,SLOT(emitExit()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_glImageRenderer;
}

void MainWindow::setUniforms( unsigned int index )
{
  foreach( QSharedPointer< ShaderParameterControlHandle > control,
      m_shaderParameterBundle.getShaderParameterControls( index ) )
  {
    control->setParameterFromControls(
        m_shaderParameterBundle.getShaderProgram( index ) );
  }
}

void MainWindow::emitExit()
{
    emit exitProgram();
}

void MainWindow::loadShaderDialog()
{
  // FIXME: Could use dependency injection, but this is not the most urgent
  // issue.
  ShaderParameterControlFactory factory;

  QString s = QFileDialog::getOpenFileName(this,"Load Shader");
  std::cout << s.toStdString() << std::endl;
  if(s != QString::null)
  {
    m_glImageRenderer->makeCurrent();
    Shader* shader = compiler.compileFile(QGLShader::Fragment,s);
    m_glImageRenderer->doneCurrent();

    if(compiler.success())
    {
      QSharedPointer<Shader> shaderPointer = 
          QSharedPointer<Shader>( shader );

      QWidget *controlWidget = new QWidget();
      QList< QSharedPointer< ShaderParameterControlHandle > > 
          shaderParameterControls;

      if(!m_glImageRenderer)
        std::cout << "glImageRenderer is NULL!" << std::endl;

      factory.generateControls( QSharedPointer< Shader >(shaderPointer), 
          m_glImageRenderer, *controlWidget, shaderParameterControls );

      m_shaderParameterBundle.append( shaderPointer, 
          shaderParameterControls );

      QScrollArea *scrollArea = new QScrollArea();
      scrollArea->setWidget( controlWidget );

      ui->stackedWidget_ShaderOptions->addWidget( scrollArea );
      ui->listView_ShaderList->addShader(shaderPointer);
    } 
    else 
    {
      QMessageBox::warning(this, tr("An Error accured while compiling"),
                                      compiler.log(),
                                      QMessageBox::Ok);
    }
  }
}

void MainWindow::removeShader()
{
    ui->stackedWidget_ShaderOptions->removeWidget(
            ui->stackedWidget_ShaderOptions->currentWidget());
    ui->listView_ShaderList->removeSelectedShader();
}

void MainWindow::loadImageFile()
{
    QString s = QFileDialog::getOpenFileName();

    if(s != QString::null){
        m_glImageRenderer->setSourceImage(QImage(s));
    }
}

void MainWindow::saveImage()
{
    QString s = QFileDialog::getSaveFileName(this,"Chose a file to save the image");

    if(s != QString::null){
        const QImage tmpImg = m_glImageRenderer->getRenderedImage();
        tmpImg.save(s);
    }
}

void MainWindow::render()
{
    m_glImageRenderer->setShaderProgramList(
        ui->listView_ShaderList->getCheckedShaders());
    m_glImageRenderer->renderImage();
}

void MainWindow::showControls( int index )
{
  ui->stackedWidget_ShaderOptions->setCurrentIndex( index );
  ui->stackedWidget_ShaderOptions->show();
}
