cmake_minimum_required(VERSION 3.0.0)
project(OpenGLProject VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(vendor)
set(SOURCE_FILES src/main.cpp src/mesh.cpp src/common.cpp src/font.cpp) 
add_executable(OpenGLProject ${SOURCE_FILES})

target_include_directories(OpenGLProject PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(OpenGLProject glfw freetype glm stbi_image_h glad)