//
// Created by Karol Hetman on 24/02/2023.
//

#ifndef EDULAB_CORE_H
#define EDULAB_CORE_H

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
#define DEFAULT_FPS 60

#include "box2d/box2d.h"
#include "vector"
#include "SDL2/SDL.h"


#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <stdio.h>
//#include <SDL.h>
#include "GUI.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif


#endif //EDULAB_CORE_H
