#ifndef IMGWRAPPER_HPP
#define IMGWRAPPER_HPP

#include <D3DX11tex.h>

namespace icons
{
    inline ID3D11ShaderResourceView* fileImage          = nullptr;

    inline ID3D11ShaderResourceView* tableImage         = nullptr;

    inline ID3D11ShaderResourceView* restartImage       = nullptr;

    inline ID3D11ShaderResourceView* newDementionImage  = nullptr;
}

class ImgWrapper
{
public:
    static void InitImages(ID3D11Device* device);
};

#endif //IMGWRAPPER_HPP
