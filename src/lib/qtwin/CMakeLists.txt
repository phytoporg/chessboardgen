cmake_minimum_required(VERSION 3.7)
set(TARGET_NAME libqtwin)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(libqtwin_MOC_HEADERS RenderWidget.h MainWindow.h)

qt4_wrap_cpp(libqtwin_MOC_FILES ${libqtwin_MOC_HEADERS})
file_to_cpp(libqtwin_DATA shaders::quad shaders/quad.glsl)
file_to_cpp(libqtwin_DATA shaders::raytracer shaders/raytracer.glsl)

file(GLOB SOURCES *.cpp)
add_library(${TARGET_NAME} ${SOURCES} ${libqtwin_DATA} ${libqtwin_MOC_FILES})
