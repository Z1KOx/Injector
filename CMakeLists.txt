cmake_minimum_required(VERSION 3.10)
project(Injector)

set(CMAKE_CXX_STANDARD 20)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/dependencies/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_impl_dx9.cpp
    ${IMGUI_DIR}/imgui_impl_win32.cpp
)

set(SOURCES
    src/main.cpp
    src/render.cpp
)

include_directories(
    ${IMGUI_DIR}
    ${CMAKE_SOURCE_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/dependencies/imgui
)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${IMGUI_SOURCES})

target_link_libraries(${PROJECT_NAME} d3d9)