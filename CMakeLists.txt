cmake_minimum_required(VERSION 3.24)
project(EduLab)

set(CMAKE_CXX_STANDARD 17)


find_package(Boost 1.75.0 REQUIRED COMPONENTS filesystem system)


if(APPLE)
    find_package(SDL2 REQUIRED)
    find_package(glfw3 REQUIRED)
else()
    find_package(OpenGL REQUIRED)
    add_subdirectory(vendor/SDL)
endif()


add_subdirectory(vendor/box2d)
add_subdirectory(vendor/glm)



include_directories(vendor/box2d/include
        vendor/imgui
        vendor/imgui/backends
        vendor/SDL/include
        vendor/glm
        src
        ${Boost_INCLUDE_DIRS}
        vendor/nlohmann
        )





add_executable(${PROJECT_NAME} main.cpp vendor/imgui/imgui.cpp vendor/imgui/imgui_demo.cpp vendor/imgui/imgui_draw.cpp vendor/imgui/imgui_internal.h vendor/imgui/imgui_tables.cpp vendor/imgui/imgui_widgets.cpp vendor/imgui/imstb_rectpack.h vendor/imgui/imstb_textedit.h vendor/imgui/imstb_truetype.h vendor/imgui/backends/imgui_impl_opengl3.cpp vendor/imgui/backends/imgui_impl_opengl3_loader.h vendor/imgui/backends/imgui_impl_glfw.cpp src/core.h src/GUI.cpp src/GUI.h vendor/imgui/backends/imgui_impl_metal.mm vendor/imgui/backends/imgui_impl_sdl2.cpp src/Scene/Scene2DManager.h src/utility.h src/Scene/Scene2DManager.cpp src/Scene/Scene2D.cpp src/Scene/Scene2D.h)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework Metal" "-framework Foundation"  "-framework QuartzCore" "-framework AppKit" "-framework opengl")
else()

    target_link_libraries(${PROJECT_NAME} opengl32 OpenGL::GL )
endif()
target_include_directories(${PROJECT_NAME} PRIVATE vendor/nlohmann)

target_link_libraries(${PROJECT_NAME} box2d  glfw imgui SDL2::SDL2 ${Boost_LIBRARIES})
