cmake_minimum_required(VERSION 3.20.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(mango)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)
add_executable(mango 
src/main.cpp
src/mango.cpp
src/mango.h
src/utils.h
src/gui/button.cpp
src/gui/button.h
src/gui/canvas.cpp
src/gui/canvas.h
src/gui/pen.cpp
src/gui/pen.h
src/gui/textentry.h
src/gui/textentry.cpp
)


target_link_libraries(mango PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2::SDL2_image  SDL2::SDL2_ttf)

set_property(TARGET mango PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
