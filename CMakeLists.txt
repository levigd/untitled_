cmake_minimum_required(VERSION 3.30)
project(untitled)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} WIN32
        main.cpp
        Gui/Gui.cpp
        Gui/Gui.hpp
        Gui/Dx11Wrapper.cpp
        Gui/Dx11Wrapper.hpp
        ImgWrapper/ImgWrapper.cpp
        ImgWrapper/ImgWrapper.hpp
        ImgWrapper/Images.hpp
)

file(GLOB IMGUI_SOURCES
        ${CMAKE_SOURCE_DIR}/Libs/ImGui/imgui.cpp
        ${CMAKE_SOURCE_DIR}/Libs/ImGui/imgui_draw.cpp
        ${CMAKE_SOURCE_DIR}/Libs/ImGui/imgui_tables.cpp
        ${CMAKE_SOURCE_DIR}/Libs/ImGui/imgui_widgets.cpp
        ${CMAKE_SOURCE_DIR}/Libs/ImGui/imgui_impl_dx11.cpp
        ${CMAKE_SOURCE_DIR}/Libs/ImGui/imgui_impl_win32.cpp
        ${CMAKE_SOURCE_DIR}/Libs/ImGui/imgui_demo.cpp
)

file(GLOB IMPLOT_SOURCES
        ${CMAKE_SOURCE_DIR}/Libs/ImPlot/implot.cpp
        ${CMAKE_SOURCE_DIR}/Libs/ImPlot/implot_items.cpp
)

target_sources(${PROJECT_NAME} PRIVATE
        ${IMGUI_SOURCES}
        ${IMPLOT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Libs/ImGui
        ${CMAKE_SOURCE_DIR}/Libs/ImPlot
        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
)

target_link_libraries(${PROJECT_NAME}
        d3d11
        dxgi
        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/d3dx11.lib"
)
