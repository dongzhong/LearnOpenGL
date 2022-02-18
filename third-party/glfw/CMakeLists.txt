cmake_minimum_required(VERSION 3.8.0)
project(glfw)

add_library(glfw INTERFACE)

# target MacOS X
if (APPLE AND NOT IOS)
    set(GLFW_PATH ${PROJECT_SOURCE_DIR}/macosx)
    target_include_directories(glfw INTERFACE ${GLFW_PATH}/include)
    target_link_libraries(glfw INTERFACE
      "${GLFW_PATH}/lib/libglfw3.a"
      "-framework OpenGL"
      "-framework IOKit"
      "-framework Cocoa"
    )
elseif (WIN32)
    set(GLFW_PATH ${PROJECT_SOURCE_DIR}/windows)
    target_include_directories(glfw INTERFACE ${GLFW_PATH}/include)
    target_link_libraries(glfw INTERFACE "${GLFW_PATH}/lib/libglfw3.a")
endif ()


