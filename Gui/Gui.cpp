#include <cstdio>
#include <D3DX11tex.h>

#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <vector>

#include "Gui.hpp"
#include "Dx11Wrapper.hpp"
#include "../ImgWrapper/ImgWrapper.hpp"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "implot.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Закругление углов + фон + отступы.
__inline static void OnDesign()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
}

// Выкл закругление углов + фон + отступы.
__inline static void OffDesign()
{
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor();
}

__inline static void DisableBackColors()
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); // Прозрачный фон в обычном состоянии
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Серый фон при наведении
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f)); // Темно-серый фон при нажатии
}

__inline static void EnableBackColors()
{
    ImGui::PopStyleColor(3);
}

__inline static void EnableMinimalButtonStyle()
{
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.10f, 0.10f, 0.10f, 1.0f));
}

__inline static void DisableMinimalButtonStyle()
{
    ImGui::PopStyleColor(3);
}

__inline static void CustomizeStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Фон + текст.
    colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);

    // Рамки.
    colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    // Заголовки.
    colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);

    // Фон активных элементов.
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);

    // Кнопки.
    colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);

    // Ползунки.
    colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);

    // Вкладки.
    colors[ImGuiCol_Tab] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

    // Полосы прокрутки.
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

    // Выделения.
    colors[ImGuiCol_CheckMark] = ImVec4(0.35f, 0.75f, 0.35f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
}

void Gui::InitImGui(HWND hWnd, Dx11Wrapper &dx11)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 22.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    ImGui::StyleColorsDark();
    CustomizeStyle();
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(dx11.GetDevice(), dx11.GetDeviceContext());
}

void Gui::MainRender(const Dx11Wrapper &dx11)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Файл"))
        {
            ImGui::MenuItem("Открыть...", "Ctrl+O");
            ImGui::MenuItem("Фильтр текущего файла");
            ImGui::MenuItem("Показать заголовок");
            ImGui::MenuItem("Открыть OSC");
            ImGui::MenuItem("Показать заголовок OSC");

            ImGui::Separator();

            ImGui::MenuItem("Создать проект...", "Alt+N");
            ImGui::MenuItem("Открыть проект...", "Alt+O");
            ImGui::MenuItem("Закрыть проект");

            ImGui::Separator();

            ImGui::MenuItem("Установка принтера");
            ImGui::MenuItem("Печать...", "Ctrl+P");
            ImGui::MenuItem("Печать протокола...");

            ImGui::Separator();

            ImGui::MenuItem("Фильтр и просмотр");
            ImGui::MenuItem("Восстановление...");
            ImGui::MenuItem("Объединение");
            ImGui::MenuItem("Экспорт данных ALD");
            ImGui::MenuItem("Экспорт данных OSC");

            ImGui::Separator();

            ImGui::MenuItem("Создать настройки...");
            ImGui::MenuItem("Открыть настройки");
            ImGui::MenuItem("Сохранить настройки");

            ImGui::Separator();

            ImGui::MenuItem("Выход", "Alt+F4");

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Вид"))
        {
            ImGui::MenuItem("Панель инструментов", nullptr, &view::toolbar);
            ImGui::MenuItem("Панель просмотра", nullptr, &view::preview_panel);
            ImGui::MenuItem("Панель страниц", nullptr, &view::page_panel);
            ImGui::MenuItem("Панель чтения файлов", nullptr, &view::file_reading_panel);
            ImGui::MenuItem("Панель калибровки", nullptr, &view::calibration_panel);
            ImGui::MenuItem("Строка состояния", nullptr, &view::status_bar);

            ImGui::Separator();

            ImGui::MenuItem("Окно проекта", nullptr, &view::project_window);
            ImGui::MenuItem("Синхр. просмотр", nullptr, &view::sync_view);
            ImGui::MenuItem("Временные маркеры", nullptr, &view::temp_markers);
            ImGui::MenuItem("Индикатор работы ППС", nullptr, &view::pps_indicator);

            ImGui::Separator();

            ImGui::MenuItem("Крупнее", nullptr, &view::zoom_in);
            ImGui::MenuItem("Мельче", nullptr, &view::zoom_out);
            ImGui::MenuItem("Назад", nullptr, &view::back);
            ImGui::MenuItem("Синхр. скроллинг", nullptr, &view::sync_scrolling);

            ImGui::Separator();

            ImGui::MenuItem("Шкала X", nullptr, &view::scale_x);
            ImGui::MenuItem("Шкала Y", nullptr, &view::scale_y);
            ImGui::MenuItem("Шкала Z", nullptr, &view::scale_z);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Управление"))
        {
            ImGui::MenuItem("Старт", "Ctrl+F5");
            ImGui::MenuItem("Стоп", "Ctrl+F6");
            ImGui::MenuItem("Пауза", "Ctrl+F7");

            ImGui::Separator();

            ImGui::MenuItem("Звук", nullptr, &management::sound);
            ImGui::MenuItem("Автопорог");
            ImGui::MenuItem("Режим настройки ППС");

            ImGui::Separator();

            ImGui::MenuItem("Старт OSC", "Ctrl+Shift+F5");
            ImGui::MenuItem("Стоп OSC", "Ctrl+Shift+F6");
            ImGui::MenuItem("Запись OSC", "Ctrl+R");

            ImGui::Separator();

            ImGui::MenuItem("Новое измерение", "Ctrl+N");
            ImGui::MenuItem("Перезапуск", "Ctrl+L");
            ImGui::MenuItem("Маркер", "Ctrl+M");

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Локация"))
        {
            ImGui::MenuItem("Новая локация");
            ImGui::MenuItem("Открыть локацию");
            ImGui::MenuItem("Закрыть локацию");

            ImGui::Separator();

            ImGui::MenuItem("Очистка окон локации");
            ImGui::MenuItem("Восстановление окон локации");

            ImGui::Separator();

            ImGui::MenuItem("Новое измерение скорости");
            ImGui::MenuItem("Открыть измерение скорости");
            ImGui::MenuItem("Закрыть измерение скорости");

            ImGui::Separator();

            ImGui::MenuItem("Новая калибровка каналов");
            ImGui::MenuItem("Открыть калибровку каналов");
            ImGui::MenuItem("Изменить параметры калибровки");
            ImGui::MenuItem("Закрыть калибровку каналов");

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Классификация"))
        {
            ImGui::MenuItem("Амплитудный критерий");
            ImGui::MenuItem("Локально-динамический критерий");
            ImGui::MenuItem("Диаграмма S-H");
            ImGui::MenuItem("Статистический критерий");
            ImGui::MenuItem("Критерий GB/T 18182-2012");
            ImGui::MenuItem("Критерий JB/T 10764-2023");

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Настройки"))
        {
            ImGui::MenuItem("Параметры каналов");
            ImGui::MenuItem("Параметры OSC");
            ImGui::MenuItem("Временные параметры");
            ImGui::MenuItem("Параметры калибраторов");
            ImGui::MenuItem("Параметрические входы");

            ImGui::Separator();

            if (ImGui::BeginMenu("Аппаратура"))
            {
                ImGui::MenuItem("Профили аппаратуры");

                ImGui::EndMenu();
            }

            ImGui::Separator();

            ImGui::MenuItem("Текущее окно");
            ImGui::MenuItem("Горячие клавиши...");
            ImGui::MenuItem("Параметры...");

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Окно"))
        {
            ImGui::MenuItem("Менеджер страниц");
            ImGui::MenuItem("Менеджер тревог");
            ImGui::MenuItem("Менеджер окон");
            ImGui::MenuItem("Линии нагружения");

            ImGui::Separator();

            ImGui::MenuItem("Восстановить все");
            ImGui::MenuItem("Каскадом");
            ImGui::MenuItem("Мозаикой");
            ImGui::MenuItem("Упорядочить значки");

            ImGui::Separator();

            if (ImGui::MenuItem("1 Средняя амплитуда (дБ) / Время (с)", nullptr, window::active_mode == 1))
                window::active_mode = 1;

            if (ImGui::MenuItem("2 Активность импульсов АЗ / Время (с)", nullptr, window::active_mode == 2))
                window::active_mode = 2;

            if (ImGui::MenuItem("3 Суммарный счет импульсов АЗ / Время (с)", nullptr, window::active_mode == 3))
                window::active_mode = 3;

            if (ImGui::MenuItem("4 Амплитуда (дБ) / Время (с)", nullptr, window::active_mode == 4))
                window::active_mode = 4;

            if (ImGui::MenuItem("5 Среднее время нарастания (мкс) / Время (с)", nullptr, window::active_mode == 5))
                window::active_mode = 5;

            if (ImGui::MenuItem("6 Средняя длительность (мкс) / Время (с)", nullptr, window::active_mode == 6))
                window::active_mode = 6;

            if (ImGui::MenuItem("7 Амплитуда шума (дБ) / Канал", nullptr, window::active_mode == 7))
                window::active_mode = 7;

            if (ImGui::MenuItem("8 Среднее число выбросов / Время (с)", nullptr, window::active_mode == 8))
                window::active_mode = 8;

            if (ImGui::MenuItem("9 Скорость счета / Время (с)", nullptr, window::active_mode == 9))
                window::active_mode = 9;

            ImGui::Separator();

            ImGui::MenuItem("Все окна...");

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Помощь"))
        {
            ImGui::MenuItem("Содержание");
            ImGui::MenuItem("Список тем");

            ImGui::Separator();

            ImGui::MenuItem("О программе");

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (view::toolbar)
    {
        RenderToolbar(dx11);
    }

    g_Gui.ShowAmplitudeTimePlot();

    OnDesign();
    DisableBackColors();

    RenderGraphics(dx11);

    if (view::preview_panel)
    {
        ImGui::Begin("Панель управления", &view::page_panel, ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Входы");
        static bool channels[8] = {true, false, true, true, false, true, false, true};
        for (int i = 0; i < 8; ++i)
        {
            char label[32];
            sprintf_s(label, "Канал %d", i + 1);
            ImGui::Checkbox(label, &channels[i]);
        }
        ImGui::Separator();
        ImGui::Text("Дополнительно");
        ImGui::Button("Фильтр");
        ImGui::Button("Настройки OSC");
        ImGui::End();
    }
    OffDesign();
    EnableBackColors();
    ImGui::Render();
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    float clear_color_with_alpha[4] =
    {
        clear_color.x * clear_color.w,
        clear_color.y * clear_color.w,
        clear_color.z * clear_color.w,
        clear_color.w
    };

    auto *ctx = dx11.GetDeviceContext();
    auto *rtv = dx11.GetMainRenderTargetView();
    ctx->OMSetRenderTargets(1, &rtv, nullptr);
    ctx->ClearRenderTargetView(rtv, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    dx11.GetSwapChain()->Present(1, 0);
}

void Gui::RenderToolbar(const Dx11Wrapper &dx11)
{
    static bool t = false;

    if (!t)
    {
        ImgWrapper::InitImages(dx11.GetDevice());
        t = true;
    }

    if (ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar))
    {
        ImGui::SetWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
        ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 42));

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); // Прозрачный фон в обычном состоянии
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Серый фон при наведении
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f)); // Темно-серый фон при нажатии

        if (ImGui::ImageButton("##FileButton", reinterpret_cast<ImTextureID>(icons::fileImage), ImVec2(18, 18), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)))
        {
            //
        }

        ImGui::SameLine();

        if (ImGui::ImageButton("##FileButton2", reinterpret_cast<ImTextureID>(icons::tableImage), ImVec2(18, 18), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)))
        {
            //
        }

        ImGui::SameLine();

        if (ImGui::ImageButton("##FileButton3", reinterpret_cast<ImTextureID>(icons::restartImage), ImVec2(18, 18), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)))
        {
            //
        }

        ImGui::SameLine();

        if (ImGui::ImageButton("##FileButton4", reinterpret_cast<ImTextureID>(icons::newDementionImage), ImVec2(18, 18), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)))
        {
            //
        }

        ImGui::SameLine();

        ImGui::PopStyleColor(3);

        ImGui::End();
    }
}

void Gui::RenderGraphics(const Dx11Wrapper &dx11)
{
    static float data1[50] = {0.2f, 0.4f, 0.8f, 0.6f, 0.7f, 0.5f, 0.9f, 1.0f, 0.3f, 0.4f};
    static float data2[50] = {0.1f, 0.3f, 0.2f, 0.6f, 0.8f, 0.9f, 0.5f, 0.4f, 0.3f, 0.7f};
    static float data3[50] = {1.0f, 0.9f, 0.8f, 0.7f, 0.6f, 0.5f, 0.4f, 0.3f, 0.2f, 0.1f};

    if (graphics::showGraph1)
    {
        ImGui::Begin("Средняя амплитуда (дБ) / Время (с)", &graphics::showGraph1);
        ImGui::Text("График: Средняя амплитуда");
        ImGui::PlotLines("Amplitude", data1, IM_ARRAYSIZE(data1), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
        ImGui::End();
    }

    if (graphics::showGraph2)
    {
        ImGui::Begin("Активность импульсов АЗ / Время (с)", &graphics::showGraph2);
        ImGui::Text("График: Активность импульсов");
        ImGui::PlotHistogram("Activity", data2, IM_ARRAYSIZE(data2), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
        ImGui::End();
    }

    if (graphics::showGraph3)
    {
        ImGui::Begin("Суммарный счет импульсов АЗ / Время (с)", &graphics::showGraph3);
        ImGui::Text("График: Суммарный счет импульсов");
        ImGui::PlotLines("Impulse Sum", data3, IM_ARRAYSIZE(data3), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
        ImGui::End();
    }

    if (graphics::showGraph4)
    {
        ImGui::Begin("Амплитуда (дБ) / Время (с)", &graphics::showGraph4);
        ImGui::Text("График: Амплитуда");
        ImGui::PlotLines("Amplitude", data1, IM_ARRAYSIZE(data1), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
        ImGui::End();
    }

    if (graphics::showGraph5)
    {
        ImGui::Begin("Среднее время нарастания (мкс) / Время (с)", &graphics::showGraph5);
        ImGui::Text("График: Среднее время нарастания");
        ImGui::PlotLines("Rise Time", data2, IM_ARRAYSIZE(data2), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
        ImGui::End();
    }

    if (graphics::showGraph6)
    {
        ImGui::Begin("Средняя длительность (мкс) / Время (с)", &graphics::showGraph6);
        ImGui::Text("График: Средняя длительность");
        ImGui::PlotLines("Duration", data3, IM_ARRAYSIZE(data3), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
        ImGui::End();
    }

    if (graphics::showGraph7)
    {
        ImGui::Begin("Амплитуда шума (дБ) / Канал", &graphics::showGraph7);
        ImGui::Text("График: Амплитуда шума");
        ImGui::PlotHistogram("Noise Amplitude", data1, IM_ARRAYSIZE(data1), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
        ImGui::End();
    }

    if (graphics::showGraph8)
    {
        ImGui::Begin("Скорость счета / Время (с)", &graphics::showGraph8);
        ImGui::Text("График: Скорость счета");
        ImGui::PlotLines("Count Speed", data2, IM_ARRAYSIZE(data2), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
        ImGui::End();
    }
}

void Gui::ShowAmplitudeTimePlot()
{
    OnDesign();
    DisableBackColors();
    EnableMinimalButtonStyle();

    static const int POINT_COUNT = 100;
    static bool dataInitialized = false;

    static std::vector<float> timeData(POINT_COUNT);
    static std::vector<float> amplitudeData(POINT_COUNT);

    static ImVec4 lineColor = ImVec4(0.2078f, 0.5176f, 0.8588f, 1.0f);
;

    static int currentPlotType = 0;

    if (!dataInitialized)
    {
        for (int i = 0; i < POINT_COUNT; i++)
        {
            float t = static_cast<float>(i) * 0.01f;
            timeData[i]      = t;
            amplitudeData[i] = 20.0f + 10.0f * std::sinf(2.0f * M_PI * t);
        }
        dataInitialized = true;
    }

    ImGui::Begin("Amplitude vs. Time");

    const char* plotTypes[] = { "Линейный", "Точечный", "Столбчатый" };
    ImGui::Combo("Тип графика", &currentPlotType, plotTypes, IM_ARRAYSIZE(plotTypes));

    ImGui::ColorEdit3("Цвет линии", &lineColor.x, ImGuiColorEditFlags_NoInputs);

    if (ImPlot::BeginPlot("Амплитуда / Время", ImVec2(1200, 800)))
    {
        ImPlot::SetupAxes("Время (с)", "Амплитуда (дБ)");

        constexpr float lineWidth = 2.0f;
        ImPlot::SetNextLineStyle(lineColor, lineWidth);

        switch (currentPlotType)
        {
            case 0:
                ImPlot::PlotLine("Сигнал", timeData.data(), POINT_COUNT);
                break;
            case 1:
                ImPlot::PlotScatter("Сигнал", timeData.data(), POINT_COUNT);
                break;
            case 2:
            {
                std::vector<float> indices(POINT_COUNT);
                for (int i = 0; i < POINT_COUNT; ++i)
                    indices[i] = static_cast<float>(i);
                ImPlot::PlotBars("Сигнал", indices.data(), POINT_COUNT);
                break;
            }
            default:
                currentPlotType = 0;
                break;
        }

        ImPlot::EndPlot();
    }

    EnableBackColors();
    OffDesign();
    DisableMinimalButtonStyle();

    ImGui::End();
}
