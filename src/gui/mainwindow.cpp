#include "mainwindow.h"
#include "ui_mainwindow.h"


using namespace gui;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // set working directory to execution directory
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    // clear the set
    m_shaderProgramIDs = QList<int>();

    ui->setupUi(this);

    m_glImageRenderer = new GLImageRenderer(this);


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
            ui->listView_ShaderList,SLOT(removeSelectedShader()));

    connect(ui->listView_ShaderList,SIGNAL(
              renderShaderList(QList<QSharedPointer<Shader> >)),
            m_glImageRenderer,SLOT(
              renderImage(QList<QSharedPointer<Shader> >)));

    connect(ui->listView_ShaderList, SIGNAL(
              shaderClicked(QSharedPointer<Shader>)),
            this, SLOT(
              showControls(QSharedPointer<Shader>)));

    connect(ui->action_Exit,SIGNAL(triggered()),this,SLOT(emitExit()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_glImageRenderer;
}

void MainWindow::emitExit()
{
    emit exitProgram();
}

void MainWindow::loadShaderDialog()
{
    QString s = QFileDialog::getOpenFileName(this,"Load Shader");
    std::cout << s.toStdString() << std::endl;
    if(s != QString::null){
        m_glImageRenderer->makeCurrent();
        Shader* shader = compiler.compileFile(QGLShader::Fragment,s);
        m_glImageRenderer->doneCurrent();
        if(compiler.success()){
            QSharedPointer<Shader> shaderPointer = QSharedPointer<Shader>(shader);
            ui->listView_ShaderList->addShader(shaderPointer);

        } else {
            QMessageBox::warning(this, tr("An Error accured while compiling"),
                                           compiler.log(),
                                           QMessageBox::Ok);
        }
    }
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

void MainWindow::showControls(QSharedPointer<Shader> aslShaderProgram)
{
  int ref = (int) &(*aslShaderProgram);
  if( !m_shaderProgramIDs.contains( ref ) )
  {
    m_shaderProgramIDs.append( ref );

    QVBoxLayout *vBoxLayout = new QVBoxLayout();
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setLayout( vBoxLayout );

    foreach(asl::ShaderParameterInfo info, aslShaderProgram->parameters())
    {
      switch( info.type->type() )
      {
      case asl::GLTypeInfo::FLOAT:
        {
        ShaderParameterControl<QDoubleSpinBox, GLfloat> *control = 
          new ShaderParameterControl<QDoubleSpinBox, GLfloat>( info, aslShaderProgram);
        vBoxLayout->addWidget( &control->widget() );
        break;
        }

      case asl::GLTypeInfo::INT:
        {
        ShaderParameterControl<QSpinBox, GLint> *control = 
          new ShaderParameterControl<QSpinBox, GLint>( info, aslShaderProgram);
        vBoxLayout->addWidget( &control->widget() );
        break;
        }

      case asl::GLTypeInfo::UINT:
        {
        ShaderParameterControl<QSpinBox, GLuint> *control = 
          new ShaderParameterControl<QSpinBox, GLuint>( info, aslShaderProgram);
        vBoxLayout->addWidget( &control->widget() );
        break;
        }

      case asl::GLTypeInfo::BOOL:
        {
        ShaderParameterControl<QSpinBox, GLuint> *control = 
          new ShaderParameterControl<QSpinBox, GLuint>( info, aslShaderProgram);
        vBoxLayout->addWidget( &control->widget() );
        //ShaderParameterControl<QCheckBox, GLuint> *control = 
          //new ShaderParameterControl<QCheckBox, GLuint>( info, aslShaderProgram);
        break;
        }

      default:
        {
        ShaderParameterControl<QSpinBox, GLint> *control = 
          new ShaderParameterControl<QSpinBox, GLint>( info, aslShaderProgram);
        vBoxLayout->addWidget( &control->widget() );
        }
      }
    }

    int index = ui->stackedWidget_ShaderOptions->addWidget( scrollArea );
    assert(index == m_shaderProgramIDs.indexOf(ref));
  }

  ui->stackedWidget_ShaderOptions->setCurrentIndex( m_shaderProgramIDs.indexOf(ref) );
}
