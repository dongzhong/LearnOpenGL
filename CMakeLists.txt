cmake_minimum_required(VERSION 3.8.0)
project(opengl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third-party/glad)
add_subdirectory(third-party/glfw)
add_subdirectory(third-party/glm)
add_subdirectory(third-party/imgui)

file(GLOB_RECURSE SRC "src/*" "src/**/*")

add_executable(opengl ${SRC})

target_link_libraries(opengl PUBLIC glad glfw glm imgui)