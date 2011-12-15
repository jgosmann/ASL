#ifndef CHECKBOXWRAPPER_H
#define CHECKBOXWRAPPER_H

#include <qgl.h>

#include <QCheckBox>


namespace gui
{

  class CheckBoxWrapper : public QCheckBox
  {
    Q_OBJECT

  public:
    CheckBoxWrapper()
    {
      connect(this, SIGNAL( stateChanged(int) ),
          this, SLOT( emitValueChanged(int) ));
    }
    ~CheckBoxWrapper() {}

    GLuint value() const
    {
      GLuint state;

      switch( checkState() )
      {
      case Qt::Unchecked:
        state = 0; // false
        break;

      case Qt::PartiallyChecked:
        // pass through to default case
      case Qt::Checked:
        state = 1; // true
        break;
      }

      return state;
    }

    void setValue(GLuint value)
    {
      if( value == 1 ) // true
        setCheckState( Qt::Checked );
      else // false
        setCheckState( Qt::Unchecked );
    }

    /**
     * This treats the special case when the minimum is equal to the maximum.
     * This might occur when the user wants to "disable" a boolean uniform. 
     * The initial value will be taken from the minimum that is either == 0
     * or != 0.
     */
    void setRange(GLuint min, GLuint max)
    {
      if( min == max )
      {
        if( min == 0 )
          setCheckState( Qt::Unchecked );
        else // FIXME: unprecise! improvement: if( min == 1 )
          setCheckState( Qt::Checked );

        setCheckable( false );
      }
    }


  public slots:
    void emitValueChanged(int state)
    {
      Q_UNUSED(state)
      emit valueChanged();
    }


  signals:
    void valueChanged();

  };

} // namespace gui

#endif /* CHECKBOXWRAPPER_H */
