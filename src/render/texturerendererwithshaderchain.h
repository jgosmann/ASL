#ifndef TEXTURERENDERERWITHSHADERCHAIN_H
#define TEXTURERENDERERWITHSHADERCHAIN_H

#include "renderable.h"

namespace render
{
class TextureRendererWithShaderChain : public Renderable
{
public:
    TextureRendererWithShaderChain();

    void render();
};
}

#endif // TEXTURERENDERERWITHSHADERCHAIN_H
