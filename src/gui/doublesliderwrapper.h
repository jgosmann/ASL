#ifndef DOUBLESLIDERWRAPPER_H
#define DOUBLESLIDERWRAPPER_H

#include <math.h>
#include <GL/gl.h>

#include <QSlider>


namespace gui
{

  class DoubleSliderWrapper : public QSlider
  {
    Q_OBJECT

  public:
    DoubleSliderWrapper() 
    {
      connect(this, SIGNAL( valueChanged(int) ),
          this, SLOT( emitValueChanged(int) ));
    }
    ~DoubleSliderWrapper() {}

    GLfloat value()
    {
      GLfloat position = static_cast<GLfloat>( value() ) 
          * pow( 10.0f, -m_precision_);
      return static_cast<GLfloat>( position );
    }

    void setValue(GLfloat value)
    {
      setSliderPosition( static_cast<int>( value * pow( 10.0f, m_precision_ ) ) );
    }

    void setRange(GLfloat min, GLfloat max)
    {
      setRange( static_cast<int>( min * pow( 10.0f, m_precision_ ) ), 
          static_cast<int> ( max * pow( 10.0f, m_precision_ ) ) );
    }

  public slots:
    void emitValueChaned(int value)
    {
      Q_UNUSED(value)
      emit valueChanged();
    }

  signals:
    void valueChanged();


  private:
    // number of decimal places
    static const GLfloat m_precision_ = 2.0f;

  };

} // namespace gui

#endif /* DOUBLESLIDERWRAPPER_H */
