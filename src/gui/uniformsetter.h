#ifndef UNIFORMSETTER_H
#define UNIFORMSETTER_H

namespace gui
{
  typedef class asl::AnnotatedGLShaderProgram Shader;
  
  class UniformSetter
  {
  
  public:
    virtual ~UniformSetter() {}

    virtual void setUniforms( unsigned short int );

  };

} // namespace gui

#endif /* UNIFORMSETTER_H */
