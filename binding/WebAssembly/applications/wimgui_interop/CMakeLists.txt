cmake_minimum_required(VERSION 3.11.0)

add_executable( wimgui_interop
    3rdparty/imgui/imgui.h
    3rdparty/imgui/imgui.cpp
    3rdparty/imgui/imgui_demo.cpp
    3rdparty/imgui/imgui_draw.cpp
    3rdparty/imgui/imgui_widgets.cpp
    3rdparty/imgui/imgui_impl_sdl.h
    3rdparty/imgui/imgui_impl_sdl.cpp
    3rdparty/imgui/imgui_impl_opengl3.h
    3rdparty/imgui/imgui_impl_opengl3.cpp
    wimgui_interop/src/wimgui_interop.cpp
)

target_include_directories( wimgui_interop PUBLIC wimgui_interop/src)
include_directories( wimgui_interop SYSTEM PUBLIC 3rdparty/imgui ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
target_link_libraries(wimgui_interop libULIS ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
target_link_libraries(wimgui_interop "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/resources@/resources" )
