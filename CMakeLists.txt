cmake_minimum_required(VERSION 3.10)
project(Injector)

# Ensure the project is built only on Windows
if (NOT WIN32)
    message(FATAL_ERROR "This project can only be built on Windows.")
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Store project name in a variable
set(PROJECT_NAME injector)

# Define IMGUI directory and sources
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

# Define project sources
set(SOURCES
    src/main.cpp
    src/render.cpp
    src/gui.cpp
    src/fileDialog.cpp
    src/process.cpp
    src/injector.cpp
)

# Include directories for the project
include_directories(
    ${IMGUI_DIR}
    ${CMAKE_SOURCE_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/dependencies/imgui
)

# Add executable
add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${IMGUI_SOURCES})

# Link required libraries
target_link_libraries(${PROJECT_NAME} d3d9 kernel32)

# Set compile features
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)