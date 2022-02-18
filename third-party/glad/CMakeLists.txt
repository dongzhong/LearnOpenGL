cmake_minimum_required(VERSION 3.8.0)
project(glad)

file(GLOB_RECURSE SRC "src/*" "src/**/*")

add_library(glad STATIC ${SRC})
target_include_directories(glad PUBLIC include)