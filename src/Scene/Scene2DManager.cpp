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




void * Scene2DManager::CreateBox(float x, float y, float width, float height, b2BodyType bodyType, ImVec4 color, float angle) {
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    bodyDef.type = bodyType;
    bodyDef.angle = angle;

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

b2Body * Scene2DManager::CreateCircle(float x, float y, float radius, b2BodyType bodyType, ImVec4 color, float angle) {
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    bodyDef.type = bodyType;
    bodyDef.angle = angle;

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

void Scene2DManager::DrawRectangle(const ImVec2 &position, const ImVec2 &size, float rotation, ImU32 color,
                                   ImDrawList* drawList) {
    //apply camera on size and position
    const Camera camera = *current_scene_->get_camera();


    ImVec2 halfSize = ImVec2(size.x * 0.5f, size.y * 0.5f);
    ImVec2 corners[4] = {
            ImVec2(-halfSize.x, -halfSize.y),
            ImVec2(halfSize.x, -halfSize.y),
            ImVec2(halfSize.x, halfSize.y),
            ImVec2(-halfSize.x, halfSize.y)
    };

    float sinRotation = std::sin(rotation);
    float cosRotation = std::cos(rotation);

    // Rotate and translate corners
    for (int i = 0; i < 4; ++i) {
        ImVec2& corner = corners[i];
        float x = corner.x * cosRotation - corner.y * sinRotation;
        float y = corner.x * sinRotation + corner.y * cosRotation;
        corner.x = x + position.x;
        corner.y = y + position.y;
    }

    //Apply camera


    // Draw rectangle
    for (int i = 0; i < 4; ++i) {
        drawList->AddLine(corners[i], corners[(i + 1) % 4], color);
    }
}

void Scene2DManager::DrawCircle(const ImVec2& position, float radius, float rotation, ImU32 color) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Draw circle
    const int num_segments = 64; // You can adjust this for smoother circles
    drawList->AddCircle(position, radius, color, num_segments);

    // Draw reference line
    ImVec2 startPoint = position;
    ImVec2 endPoint = ImVec2(position.x + radius * std::cos(rotation), position.y + radius * std::sin(rotation));
    drawList->AddLine(startPoint, endPoint, color);
}

void Scene2DManager::DrawFilledRectangle(const ImVec2& position, const ImVec2& size, float rotation, ImU32 color) {
    ImVec2 halfSize = ImVec2(size.x * 0.5f, size.y * 0.5f);;
    ImVec2 corners[4] = {
            ImVec2(-halfSize.x, -halfSize.y),
            ImVec2(halfSize.x, -halfSize.y),
            ImVec2(halfSize.x, halfSize.y),
            ImVec2(-halfSize.x, halfSize.y)
    };

    float sinRotation = std::sin(rotation);
    float cosRotation = std::cos(rotation);

    // Rotate and translate corners
    for (auto & corner : corners) {
        float x = corner.x * cosRotation - corner.y * sinRotation;
        float y = corner.x * sinRotation + corner.y * cosRotation;
        corner.x = x + position.x;
        corner.y = y + position.y;
    }

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Draw filled rectangle
    drawList->AddConvexPolyFilled(corners, 4, color);
}


void Scene2DManager::draw_scene(ImDrawList *draw_list, b2Vec2 screen) {
    Camera camera = *current_scene_->get_camera();
    for (auto &item : current_scene_->get_bodies()) {

        float screen_ratio = screen.x / screen.y;
        ImVec4 color = item.second;
        ImColor im_color = ImColor(color.x, color.y, color.z, color.w);

        b2Body* body = item.first;
        b2Vec2 position = body->GetPosition();
        position.x *= camera.zoom; // multiply by zoom
        position.y *= camera.zoom;
        position.x -= camera.x; // add camera position
        position.y += camera.y;
        position +=  screen; // add camera position
        // multiply by zoom



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
                this->DrawCircle(ImVec2(position.x, position.y), radius, angle, im_color);
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
                // transform to screen coordinates

                this->DrawRectangle(ImVec2(position.x, position.y), ImVec2(width, height), angle, im_color, draw_list);
                break;
            }
            default:
                break;
            
        }

    }
}

Scene2D *Scene2DManager::get_current_scene() {
    return current_scene_;
}

void Scene2DManager::update() {
    if (this->running){
        this->current_scene_->get_world()->Step(1.0f / 60.0f, 6, 2);
    }


}






