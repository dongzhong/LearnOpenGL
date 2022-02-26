cmake_minimum_required(VERSION 3.8.0)
project(imgui)

file(GLOB_RECURSE SRC "src/*" "src/**/*")

add_library(imgui STATIC ${SRC})
target_include_directories(imgui PUBLIC src)

target_link_libraries(imgui PUBLIC glfw)
