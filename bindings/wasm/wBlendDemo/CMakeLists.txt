cmake_minimum_required(VERSION 3.11.0)

add_executable( wBlendDemo
    3rdparty/imgui/imgui.h
    3rdparty/imgui/imgui.cpp
    3rdparty/imgui/imgui_demo.cpp
    3rdparty/imgui/imgui_draw.cpp
    3rdparty/imgui/imgui_widgets.cpp
    3rdparty/imgui/imgui_impl_sdl.h
    3rdparty/imgui/imgui_impl_sdl.cpp
    3rdparty/imgui/imgui_impl_opengl3.h
    3rdparty/imgui/imgui_impl_opengl3.cpp
    wBlendDemo/src/wBlendDemo.cpp
)

target_include_directories( wBlendDemo PUBLIC wBlendDemo/src)
include_directories( wBlendDemo SYSTEM PUBLIC 3rdparty/imgui ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
target_link_libraries(wBlendDemo libwULIS3 ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
target_link_libraries(wBlendDemo "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/resources@/resources" )
