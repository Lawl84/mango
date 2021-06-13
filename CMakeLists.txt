cmake_minimum_required(VERSION 3.20.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(mango)

find_package(SDL2 CONFIG REQUIRED)

add_executable(mango src/main.cpp)

target_link_libraries(mango PRIVATE SDL2::SDL2 SDL2::SDL2main)

set_property(TARGET mango PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})