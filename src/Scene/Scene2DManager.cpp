//
// Created by Karol Hetman on 04/03/2023.
//

#include "Scene2DManager.h"


void Scene2DManager::AddScene(Scene2D *scene) {
    m_scenes.push_back(scene);

}

void Scene2DManager::RemoveScene(int index) {
    delete m_scenes[index];
    m_scenes.erase(m_scenes.begin() + index);

}

void Scene2DManager::SetCurrentScene(Scene2D* index) {
    m_currentScene = index;

}

Scene2DManager::Scene2DManager() {
    m_currentScene = nullptr;

    m_window = SDL_CreateWindow("EduLab", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (m_window == nullptr) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    }
}

void Scene2DManager::RenderCurrentScene() {
    //Clear screen
    SDL_SetRenderDrawColor( m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
    SDL_RenderClear( m_renderer );

    //Render objects
    for (auto& object : m_currentScene->GetObjects()) {
        std::vector<SDL_Vertex> vertices = object->GetVertices();
        SDL_RenderGeometry( m_renderer, nullptr, vertices.data(), vertices.size(), nullptr, 0 );

    }
    //Update screen
    SDL_RenderPresent( m_renderer );

}

Scene2DManager::~Scene2DManager() {
    for (auto& scene : m_scenes) {
        delete scene;
    }
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    m_renderer = nullptr;
    m_window = nullptr;
    SDL_Quit();
}
