//
// Created by Karol Hetman on 27/02/2023.
//

#ifndef EDULAB_GUI_H
#define EDULAB_GUI_H
#include "core.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif





class GUI {
public:

    explicit GUI(SDL_Window* window, SDL_GLContext glContext,const char* glslVersion,SDL_Event* event);
    ~GUI();
    int Init();
    void Exit();
    void Render();
    void PollEvents();
    void CreateWindow();
    bool done = false;

private:
    SDL_Window* m_window;
    SDL_GLContext m_glContext{};
    SDL_Event* m_event;
    const char* glsl_version;

    ImGuiIO m_io;
    ImGuiStyle m_style;

    // Our state
    bool m_show_demo_window = true;
    bool m_show_another_window = false;
    ImVec4 m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



};


#endif //EDULAB_GUI_H
