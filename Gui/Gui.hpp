#pragma once

#include <windows.h>
#include <cstdint>

#include "Dx11Wrapper.hpp"

// Вид
namespace view
{
    static bool toolbar            = true;
    static bool preview_panel      = true;

    static bool page_panel         = false; // TODO
    static bool file_reading_panel = false;
    static bool calibration_panel  = true;
    static bool status_bar         = true;  //TODO

    static bool project_window     = false;
    static bool sync_view          = false;
    static bool temp_markers       = false;
    static bool pps_indicator      = true;

    static bool zoom_in            = false;
    static bool zoom_out           = false;
    static bool back               = false;
    static bool sync_scrolling     = true;

    static bool scale_x            = true;
    static bool scale_y            = true;
    static bool scale_z            = false;
}

// Управление
namespace management
{
    static bool sound = true;
}

// Окна
namespace window
{
    static std::uint8_t active_mode = 0;
}

// Графики?
namespace graphics
{
    static bool showGraph1 = false;
    static bool showGraph2 = false;
    static bool showGraph3 = false;
    static bool showGraph4 = false;
    static bool showGraph5 = false;
    static bool showGraph6 = false;
    static bool showGraph7 = false;
    static bool showGraph8 = false;
}

class Dx11Wrapper;

class Gui
{
private:
    bool show_side_panel    = true;

public:
    void InitImGui(HWND hWnd, Dx11Wrapper& dx11);
    void MainRender(const Dx11Wrapper& dx11);
    void RenderToolbar(const Dx11Wrapper& dx11);
    void RenderGraphics(const Dx11Wrapper& dx11);
    void ShowAmplitudeTimePlot();
};

static Gui g_Gui;