#ifndef GLVARIANT_H
#define GLVARIANT_H

#include <gl.h>

namespace asl
{
    const unsigned int MAX_GL_VECTOR_DIMENSION = 4;

    union GLVariant
    {
    public:
        GLfloat asFloats[MAX_GL_VECTOR_DIMENSION * MAX_GL_VECTOR_DIMENSION];
        GLint asInts[MAX_GL_VECTOR_DIMENSION * MAX_GL_VECTOR_DIMENSION];

        inline void set(GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
             asFloats[0] = v0;
             asFloats[1] = v1;
             asFloats[2] = v2;
             asFloats[3] = v3;
         }

        inline void set(GLint v0, GLint v1, GLint v2, GLint v3) {
            asInts[0] = v0;
            asInts[1] = v1;
            asInts[2] = v2;
            asInts[3] = v3;
        }

        inline void set(const GLsizei count, const GLfloat *value) {
            for (unsigned short int i = 0; i < count; ++i) {
                asFloats[i] = value[i];
            }
        }

        inline void set(const GLsizei count, const GLint *value) {
            for (unsigned short int i = 0; i < count; ++i) {
                asInts[i] = value[i];
            }
        }

        inline GLVariant(GLfloat v0 = 0.0, GLfloat v1 = 0.0,
                         GLfloat v2 = 0.0, GLfloat v3 = 0.0) {
            set(v0, v1, v2, v3);
        }

        inline GLVariant(GLint v0, GLint v1 = 0, GLint v2 = 0, GLint v3 = 0) {
            set(v0, v1, v2, v3);
        }

        inline GLVariant(const GLsizei count, const GLfloat *value) {
            set(count, value);
        }

        inline GLVariant(const GLsizei count, const GLint *value) {
            set(count, value);
        }
    };
}

#endif // GLVARIANT_H