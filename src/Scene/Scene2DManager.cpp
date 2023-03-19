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

void Scene2DManager::draw_scene(ImDrawList *draw_list, b2Vec2 screen) {
    Camera camera = current_scene_->get_camera();
    for (auto &item : current_scene_->get_bodies()) {

        ImVec4 color = item.second;
        ImColor im_color = ImColor(color.x, color.y, color.z, color.w);

        b2Body* body = item.first;
        b2Vec2 position = body->GetPosition() + camera.position + screen;

        float angle = body->GetAngle();
        //get size of the box
        b2Shape* shape = body->GetFixtureList()->GetShape();
        b2Shape::Type shapeType = shape->GetType();

        float width = 0.0f;
        float height = 0.0f;
        float radius = 0.0f;
        switch (shapeType) {
            case b2Shape::e_circle:
            {
                auto* circleShape = dynamic_cast<b2CircleShape*>(shape);
                radius = circleShape->m_radius * camera.zoom;
                // add to draw list with camera applied and angle
                draw_list->AddCircleFilled(ImVec2(position.x, position.y), radius, im_color);

                break;
            }
            case b2Shape::e_polygon:
            {
                auto* polygonShape = dynamic_cast<b2PolygonShape*>(shape);
                b2AABB aabb{};
                b2Transform identityTransform{};
                identityTransform.SetIdentity();
                polygonShape->ComputeAABB(&aabb, identityTransform, 0);

                width = (aabb.upperBound.x - aabb.lowerBound.x) * camera.zoom;
                height = (aabb.upperBound.y - aabb.lowerBound.y) * camera.zoom;
                draw_list->AddRectFilled(ImVec2(position.x, -position.y), ImVec2(width, height), im_color);
                break;
            }
            default:
                break;



        draw_list->AddRectFilled(ImVec2(position.x - 1, position.y - 1), ImVec2(position.x + 100, position.y + 100), im_color);

        }

    }
}

Scene2D *Scene2DManager::get_current_scene() {
    return current_scene_;
}

void Scene2DManager::start() {


    current_scene_->get_world()->Step(1.0f / 60.0f, 8, 3);



}

void Scene2DManager::stop() {
    m_running = false;
    //delete the current scene
    delete current_scene_;
    //load the original scene
    current_scene_ = original_scene_;
     original_scene_ = nullptr;

}

