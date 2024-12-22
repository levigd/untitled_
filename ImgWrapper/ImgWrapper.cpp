#include "ImgWrapper.hpp"
#include "Images.hpp"

void ImgWrapper::InitImages(ID3D11Device* device)
{
    D3DX11_IMAGE_LOAD_INFO info{};
    ID3DX11ThreadPump* pump{};
    D3DX11CreateShaderResourceViewFromMemory(device, rawDataFileIcon, sizeof(rawDataFileIcon), &info, pump, &icons::fileImage, 0);

    D3DX11CreateShaderResourceViewFromMemory(device, rawDataTableIcon, sizeof(rawDataTableIcon), &info, pump, &icons::tableImage, 0);

    D3DX11CreateShaderResourceViewFromMemory(device, rawDataRestartIcon, sizeof(rawDataRestartIcon), &info, pump, &icons::restartImage, 0);

    D3DX11CreateShaderResourceViewFromMemory(device, rawDataNewDemention, sizeof(rawDataNewDemention), &info, pump, &icons::newDementionImage, 0);
}
