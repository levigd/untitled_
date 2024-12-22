#pragma once

#include <d3d11.h>
#include <dxgi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

class Dx11Wrapper
{
private:
    ID3D11Device*               g_pd3dDevice            = nullptr;
    ID3D11DeviceContext*        g_pd3dDeviceContext     = nullptr;
    IDXGISwapChain*             g_pSwapChain            = nullptr;
    ID3D11RenderTargetView*     g_mainRenderTargetView  = nullptr;

public:
    bool CreateDeviceD3D(HWND hWnd);
    void CleanupDeviceD3D();
    void CreateRenderTarget();
    void CleanupRenderTarget();
    void ResizeBuffers(UINT width, UINT height);

    [[nodiscard]] bool IsDeviceReady() const
    {
        return g_pd3dDevice != nullptr;
    }
    [[nodiscard]] ID3D11Device* GetDevice() const
    {
        return g_pd3dDevice;
    }
    [[nodiscard]] ID3D11DeviceContext* GetDeviceContext() const
    {
        return g_pd3dDeviceContext;
    }
    [[nodiscard]] IDXGISwapChain* GetSwapChain() const
    {
        return g_pSwapChain;
    }
    [[nodiscard]] ID3D11RenderTargetView* GetMainRenderTargetView() const
    {
        return g_mainRenderTargetView;
    }
};

static Dx11Wrapper g_DxWrapper;