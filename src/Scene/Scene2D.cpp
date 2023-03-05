//
// Created by Karol Hetman on 25/02/2023.
//

#include "Scene2D.h"

Scene2D::Scene2D(int32 width, int32 high, b2Vec2 gravity) {
    m_camera = new Camera2D(width, high);
    m_worldLocal = new b2World(gravity);
    m_windowWidth = width;
    m_windowHeight = high;
}

Scene2D::~Scene2D() {
    delete m_camera;
    delete m_worldLocal;
    for (auto& object : m_objects) {
        delete object;
    }
}

void Scene2D::AddObject(b2Vec2 position, b2Vec2 size) {
    m_objects.push_back(new Entity(position, size));
}

void Scene2D::RemoveObject(int index) {
    delete m_objects[index];
    m_objects.erase(m_objects.begin() + index);
}


std::vector<Entity *> Scene2D::GetObjects() {
    return m_objects;
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
