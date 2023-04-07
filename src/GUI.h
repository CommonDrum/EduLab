//
// Created by Karol Hetman on 27/02/2023.
//

#ifndef EDULAB_GUI_H
#define EDULAB_GUI_H
#include "core.h"
#include <filesystem>

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


enum class UI_State {
    MAIN_MENU,
    EDITOR,
    PLAY
};


class GUI {
private:

    Object2D* selectedObject = nullptr;
    const char* glsl_version;
    ImGuiIO m_io;

    Scene2DManager* m_scene2DManager;
    float delta_time = 1.0f / ImGui::GetIO().Framerate;
    bool clicked = false;
    bool show_object_properties = false;
    bool joint_creation_enabled = false;
    int joint_creation_type = 0;
    int clicks = 0;
    bool show_grid = true;
    bool editor_mode = true;
    ImVec2 pos1 = ImVec2(0,0);
    ImVec2 pos2 = ImVec2(0,0);
    bool enable_force_application = false;



public:
    void start(bool *done);
    void editor(bool *done);
    void play(bool *done);
    explicit GUI(Scene2DManager* scene2DManager);
    void tools();
    void editor(std::vector<std::string> * options);
    void object_properties_popup();
    void info();
    void file_explorer(std::string *selectedFilename);
    void menuBar(bool *done);
    ImVec2 windowCenter();
    void joint_creation();
    void force_application();

    void mainViewport();

    void main_menu(UI_State *pBoolean);

    void object_properties_popup(ImVec2 pos);
};



#endif //EDULAB_GUI_H
