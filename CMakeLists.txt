cmake_minimum_required(VERSION 3.24)
project(EduLab)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

add_subdirectory(vendor/box2d)

include_directories(vendor/box2d/include
        vendor/imgui
        vendor/imgui/backends
        )




add_executable(${PROJECT_NAME} main.cpp)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL" "-framework Metal" "-framework Foundation" "-framework QuartzCore")
else()
    target_link_libraries(${PROJECT_NAME} GL)
endif()

target_link_libraries(${PROJECT_NAME} box2d SDL2::SDL2 glfw imgui)
