//
// Created by Karol Hetman on 04/03/2023.
//

#include "Scene2DManager.h"

// Scene manager is connecting scenes with windows
// and vice versa. For 2D it will use ImGui rendering API
// with an SDL2 implementation for simplicity and compatibility.
// It receives ImGui draw list and populates it with objects from current scene,
// with sizes and positions adjusted to the current window size.
// It also handles input events and applies them to the current scene.
// It
Scene2DManager::Scene2DManager() {
    m_currentScene = nullptr;

}

Scene2DManager::~Scene2DManager() {
    for (auto& scene : m_scenes) {
        delete scene;
    }
}

void Scene2DManager::CreateScene(std::string name) {
    m_scenes.push_back(new Scene2D(name));
    if (m_currentScene == nullptr) {
        m_currentScene = m_scenes[0];
    }

}

void Scene2DManager::RemoveScene(std::string name) {
    for (auto& scene : m_scenes) {
        if (scene->GetName() == name) {
            delete scene;
            m_scenes.erase(std::remove(m_scenes.begin(), m_scenes.end(), scene), m_scenes.end());
            return;
        }
    }
    std::cout << "Scene with name " << name << " not found" << std::endl;

}

void Scene2DManager::SetCurrentScene(std::string name) {
    for (auto& scene : m_scenes) {
        if (scene->GetName() == name) {
            m_currentScene = scene;
            return;
        }
    }
    std::cout << "Scene with name " << name << " not found" << std::endl;

}

void Scene2DManager::RenderCurrentScene() {


}



