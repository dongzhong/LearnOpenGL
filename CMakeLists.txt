cmake_minimum_required(VERSION 3.8.0)
project(opengl)

file(GLOB_RECURSE SRC "src/*" "src/**/*")

add_executable(opengl ${SRC})