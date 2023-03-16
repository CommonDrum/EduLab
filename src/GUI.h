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
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

#include <Scene/Scene2DManager.h>




class GUI {
private:

    const char* glsl_version;
    ImGuiIO m_io;


public:
    void tools();
    void fileExplorer();
    void menuBar(bool *done);
    void placeholder(SDL_Window *sdl_window);

    void mainViewport(b2World *world);
};



#endif //EDULAB_GUI_H
