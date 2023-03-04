//
// Created by Karol Hetman on 25/02/2023.
//

#include "Scene2D.h"

Scene2D::Scene2D(int32 width, int32 high, b2Vec2 gravity) {
    m_camera = new Camera2D(width, high);
    m_worldLocal = new b2World(gravity);
    m_WindowWidth = width;
    m_WindowHeight = high;
}

void Scene2D::CreateContext(SDL_Window * window) {

    m_window = window;
    if (m_window == nullptr) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }
    //Create renderer for window
   m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    }

    this->Render();

}


void Scene2D::Render() {

    //Clear screen
    SDL_SetRenderDrawColor( m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
    SDL_RenderClear( m_renderer );

    //Render objects
    for (auto& object : m_objects) {
        m_vertices = object->GetVertices();
        SDL_RenderGeometry( m_renderer, nullptr, m_vertices.data(), m_vertices.size(), nullptr, 0 );

    }
    //Update screen
    SDL_RenderPresent( m_renderer );
}

Scene2D::~Scene2D() {
    delete m_camera;
    delete m_worldLocal;
    for (auto& object : m_objects) {
        delete object;
    }
    SDL_DestroyRenderer( m_renderer );
    SDL_DestroyWindow( m_window );
    SDL_Quit();
}

void Scene2D::AddObject(b2Vec2 position, b2Vec2 size) {
    m_objects.push_back(new Entity(position, size));
}

void Scene2D::RemoveObject(int index) {
    delete m_objects[index];
    m_objects.erase(m_objects.begin() + index);
}


void Scene2D::DestroyContext() {
    SDL_DestroyRenderer( m_renderer );
    SDL_DestroyWindow( m_window );
    SDL_Quit();

}



void Scene2D::Resize(int32 width, int32 height) {
    m_WindowWidth = width;
    m_WindowHeight = height;
    m_camera->m_width = width;
    m_camera->m_height = height;
}

SDL_Window *Scene2D::GetWindow() {
    return m_window;
}

/*---------------------------------------CAMERA-----------------------------------------*/

Camera2D::Camera2D(int32 width, int32 height, b2Vec2 center, float zoom) {
    m_width = width;
    m_height = height;
    m_center = center;
    m_zoom = zoom;
}

void Camera2D::ResetView() {
    m_center = b2Vec2(0.0f, 0.0f);
    m_zoom = 1.0f;
}


