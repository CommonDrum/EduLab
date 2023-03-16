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




void * Scene2DManager::CreateBox(float x, float y, float width, float height, b2BodyType bodyType, ImVec4 color) {
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    bodyDef.type = bodyType;

    b2PolygonShape shape;
    shape.SetAsBox(width / 2.0f, height / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    b2Body* body = current_scene_->get_world()->CreateBody(&bodyDef);
    body->CreateFixture(&fixtureDef);

    current_scene_->get_bodies().emplace_back(body, color);

    // return the pair
    return body;
}

b2Body * Scene2DManager::CreateCircle(float x, float y, float radius, b2BodyType bodyType, ImVec4 color) {
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    bodyDef.type = bodyType;

    b2CircleShape shape;
    shape.m_radius = radius;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    b2Body* body = current_scene_->get_world()->CreateBody(&bodyDef);
    body->CreateFixture(&fixtureDef);

    current_scene_->get_bodies().emplace_back(body, color);

    return body;
}

void Scene2DManager::draw_scene(ImDrawList *draw_list) {
    for (auto &item : current_scene_->get_bodies()) {
        ImVec4 color = item.second;
        b2Body* body = item.first;
        b2Vec2 position = body->GetPosition();
        float angle = body->GetAngle();

        draw_list->AddRectFilled(ImVec2(position.x - 1, position.y - 1), ImVec2(position.x + 1, position.y + 1), IM_COL32(255, 0, 0, 255));
        draw_list->AddRectFilled(ImVec2(position.x - 1, position.y - 1), ImVec2(position.x + 1, position.y + 1), IM_COL32(255, 0, 0, 255));
        draw_list->AddRectFilled(ImVec2(position.x - 1, position.y - 1), ImVec2(position.x + 1, position.y + 1), IM_COL32(255, 0, 0, 255));
        draw_list->AddRectFilled(ImVec2(position.x - 1, position.y - 1), ImVec2(position.x + 1, position.y + 1), IM_COL32(255, 0, 0, 255));
    }

}

