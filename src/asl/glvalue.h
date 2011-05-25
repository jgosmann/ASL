#ifndef GLVALUE_H
#define GLVALUE_H

#include <cassert>

#include <GL/gl.h>

namespace asl
{
    const GLsizei MAX_GL_VECTOR_DIMENSION = 4;

    union GLValue
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
            assert(count <= MAX_GL_VECTOR_DIMENSION * MAX_GL_VECTOR_DIMENSION);
            for (unsigned short int i = 0; i < count; ++i) {
                asFloats[i] = value[i];
            }
        }

        inline void set(const GLsizei count, const GLint *value) {
            assert(count <= MAX_GL_VECTOR_DIMENSION * MAX_GL_VECTOR_DIMENSION);
            for (unsigned short int i = 0; i < count; ++i) {
                asInts[i] = value[i];
            }
        }

        inline GLValue(GLfloat v0 = 0.0, GLfloat v1 = 0.0,
                         GLfloat v2 = 0.0, GLfloat v3 = 0.0) {
            set(v0, v1, v2, v3);
        }

        inline GLValue(GLint v0, GLint v1 = 0, GLint v2 = 0, GLint v3 = 0) {
            set(v0, v1, v2, v3);
        }

        inline GLValue(const GLsizei count, const GLfloat *value) {
            set(count, value);
        }

        inline GLValue(const GLsizei count, const GLint *value) {
            set(count, value);
        }
    };
}

#endif // GLVALUE_H
