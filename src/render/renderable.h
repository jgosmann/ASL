#ifndef RENDERABLE_H
#define RENDERABLE_H

namespace render
{
    class Renderable
    {
    public:
        virtual ~Renderable() { }

        /** Renders the object using the current \c QGLContext. */
        virtual void render() = 0;
    };
}

#endif // RENDERABLE_H
