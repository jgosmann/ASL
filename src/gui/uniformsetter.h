#ifndef UNIFORMSETTER_H
#define UNIFORMSETTER_H

namespace gui
{
  class UniformSetter
  {
  
  public:
    virtual ~UniformSetter() {}

    virtual void setUniforms( unsigned int index ) = 0;

  };

} // namespace gui

#endif /* UNIFORMSETTER_H */
