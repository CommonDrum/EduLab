//
// Created by Karol Hetman on 15/03/2023.
//
#include "Scene2DManager.h"

void Scene2DManager::create_scene(std::string name) {
    auto *new_scene = new Scene2D(std::move(name));
    if (scenes_.empty()) {
        current_scene_ = new_scene;
    }
}

std::vector<std::string> Scene2DManager::get_scene_names() const {
    std::vector<std::string> names;
    names.reserve(scenes_.size());
for (auto &scene : scenes_) {
        names.push_back(scene->get_name());
    }
    return names;
}

