cmake_minimum_required(VERSION 3.7)
set(TARGETNAME raytracer)

file(GLOB SOURCES *.cpp)
add_executable(${TARGETNAME} ${SOURCES})

include_directories(${LIB_INCLUDE_DIR})

target_link_libraries(${TARGETNAME}
    libqtwin
    ${QT_LIBRARIES}
    ${OPENGL_LIBRARIES}
    )

