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
Scene2D *Scene2DManager::get_current_scene() {
    return current_scene_;
}

void Scene2DManager::update() { // called by main.cpp
    if (this->running) {
        this->current_scene_->get_world()->Step(1.0f / 60.0f, 6, 2);
    }
}


// OBJECT CREATION ------------------------------------------------------------------------------------------------------------

void * Scene2DManager::CreateBox(float x, float y, float width, float height, b2BodyType bodyType, ImVec4 color,
                                 float angle,
                                 float density, float friction, float restitution) {
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    bodyDef.type = bodyType;
    bodyDef.angle = angle;

    b2PolygonShape shape;
    shape.SetAsBox(width / 2.0f, height / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;

    b2Body* body = current_scene_->get_world()->CreateBody(&bodyDef);
    body->CreateFixture(&fixtureDef);

    current_scene_->add_object(body, color, b2Vec2(width, height));

    // return the pair
    return body;
}

b2Body * Scene2DManager::CreateCircle(float x, float y, float radius, b2BodyType bodyType, ImVec4 color, float angle,
                                      float density, float friction, float restitution) {
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    bodyDef.type = bodyType;
    bodyDef.angle = angle;

    b2CircleShape shape;
    shape.m_radius = radius;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;

    b2Body* body = current_scene_->get_world()->CreateBody(&bodyDef);
    body->CreateFixture(&fixtureDef);

    current_scene_->add_object(body, color, b2Vec2(radius, radius));

    return body;
}
// DRAWING FUNCTIONS --------------------------------------------------------------------------------------------------------------
void Scene2DManager::DrawRectangle(const ImVec2 &position, const ImVec2 &size, float rotation, ImU32 color,ImDrawList* drawList) {

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


void Scene2DManager::draw_scene(ImDrawList *draw_list) {
    Camera camera = *current_scene_->get_camera();
    if(current_scene_->get_objects().empty()) return;
    for (auto &item : current_scene_->get_objects()) {

        energy_as_color(item);
        ImColor im_color = item->get_color();
        b2Body* body = item->get_body();

        // Extract data from body
        ImVec2 screenPosition = world_to_screen(body->GetPosition());
        float angle = body->GetAngle();
        b2Shape* shape = body->GetFixtureList()->GetShape();
        b2Shape::Type shapeType = shape->GetType();


        // Declare variables
        float width = 0.0f;
        float height = 0.0f;
        float radius = 0.0f;

        // Add to draw list depending on shape type
        switch (shapeType) {
            case b2Shape::e_circle:
            {
                auto* circleShape = dynamic_cast<b2CircleShape*>(shape);
                radius = circleShape->m_radius * camera.zoom;
                this->DrawCircle(screenPosition, radius, angle, im_color);
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

                this->DrawRectangle(screenPosition, ImVec2(width, height), angle, im_color, draw_list);
                break;
            }
            default:
                break;
            
        }

        if (item->is_showing_velocity()){
            draw_velocity( draw_list,item);
        }
        if (item->is_showing_forces()){
            draw_forces(draw_list, item);
        }
    }
}

// INTERACTION WITH THE WORLD ------------------------------------------------------------
Object2D *Scene2DManager::object_at_point(b2Vec2 point) {
    for (auto &item : current_scene_->get_objects()) {
        b2Body* body = item->get_body();
        b2Fixture *fixture = body->GetFixtureList();
        if (fixture->TestPoint(point)){
            return item;
        }

    }
    return nullptr;
}

b2Vec2 Scene2DManager::screen_to_world(const ImVec2 &screenCoords) {
    Camera camera = *current_scene_->get_camera();
    b2Vec2 worldCoords = b2Vec2(screenCoords.x, screenCoords.y);

    worldCoords.x -= camera.sX;
    worldCoords.y -= camera.sY;

    worldCoords.x += camera.x;
    worldCoords.y -= camera.y;

    worldCoords.x /= camera.zoom;
    worldCoords.y /= camera.zoom;


    return worldCoords;
}

ImVec2 Scene2DManager::world_to_screen(const b2Vec2 &worldCoords) {
    Camera camera = *current_scene_->get_camera();
    ImVec2 screenCoords = ImVec2(worldCoords.x, worldCoords.y);

    screenCoords.x *= camera.zoom;
    screenCoords.y *= camera.zoom;

    screenCoords.x -= camera.x;
    screenCoords.y += camera.y;

    screenCoords.x +=  camera.sX;
    screenCoords.y +=  camera.sY;


    return screenCoords;
}

void Scene2DManager::highlight_object_click(b2Vec2 point) {

    Object2D *object = object_at_point(point);
    if (object != nullptr){
        this->highlighted_object_ = object;
    }
    else{
        if (this->highlighted_object_ != nullptr)
            this->highlighted_object_ = nullptr;
    }


}

void Scene2DManager::move_highlighted_object(b2Vec2 point) { // for edit mode
    if (this->highlighted_object_ != nullptr && !running){
        float angle = this->highlighted_object_->get_body()->GetAngle();
        this->highlighted_object_->get_body()->SetTransform(point,angle);
    }


}

void Scene2DManager::attach_mouse_joint(b2Vec2 point) { // for play mode
     if (running)
        current_scene_->MouseDown(point);

}



void Scene2DManager::detach_mouse_joint() {
    if (running)
            current_scene_->MouseUp();

}


void Scene2DManager::move_mouse_joint(b2Vec2 point) {
    if (running)
        current_scene_->MouseMove(point);

}

void Scene2DManager::resize_highlighted_object(b2Vec2 newSize) {
    if (this->highlighted_object_ != nullptr && !running){
        highlighted_object_->set_size(newSize);
    }
}

void Scene2DManager::rotate_highlighted_object(float angle) {
    if (this->highlighted_object_ != nullptr && !running){
        highlighted_object_->rotate(angle);
    }

}

void Scene2DManager::reset_scene() {


}

void Scene2DManager::save_scene(std::string name) {
    bool is_running = running;
    running = false;
    current_scene_->serialize(name);
    running = is_running;


}

void Scene2DManager::load_scene(std::string name) {
    bool is_running = running;
    running = false;
    current_scene_->deserialize(name);
    running = is_running;

}



void Scene2DManager::draw_forces(ImDrawList *drawList, Object2D *object) {
    if(object == nullptr)
        return;

        std::vector<b2Vec2> forces;
        forces = object->get_forces();
        // print the forces
        for (auto &force: forces) {
            if (force.Length() < 0.9) continue;
            force *= 0.1;
            b2Vec2 position = object->get_body()->GetPosition();
            b2Vec2 arrow_tip = (position + force);

            ImVec2 arrow_screen = this->world_to_screen(arrow_tip);
            ImVec2 position_screen = this->world_to_screen(object->get_body()->GetPosition());
            ImU32 color = IM_COL32(255, 0, 0, 255);
            this->draw_arrow(position_screen, arrow_screen, color, drawList);
        }
    }


void Scene2DManager::draw_arrow(const ImVec2 &position, const ImVec2 &size, ImU32 color, ImDrawList *drawList) {

    ImVec2 p1 = position;
    ImVec2 p2 = size;

    drawList->AddLine(p1, p2, color, 2.0f);

    // calculate the angle of the line
    float angle = atan2(p1.y - p2.y, p1.x - p2.x);
    // starting point of the first line of the arrow head
    ImVec2 p3 = ImVec2(p2.x + 6 * cos(angle + M_PI / 6), p2.y + 6 * sin(angle + M_PI / 6));
    // starting point of the second line of the arrow head
    ImVec2 p4 = ImVec2(p2.x + 6 * cos(angle - M_PI / 6), p2.y + 6 * sin(angle - M_PI / 6));
    // draw triangle
    drawList->AddTriangleFilled(p2, p3, p4, color);
}

void Scene2DManager::draw_velocity(ImDrawList *drawList, Object2D *object) {
    if(object == nullptr) return;

    b2Vec2 velocity = object->get_velocity();
    if (velocity.Length() < 0.9) return;


    velocity *= 0.1;
    b2Vec2 position = object->get_body()->GetPosition();
    b2Vec2 arrow_tip = (position + velocity);

    ImVec2 arrow_screen = this->world_to_screen(arrow_tip);
    ImVec2 position_screen = this->world_to_screen(object->get_body()->GetPosition());
    ImU32 color = IM_COL32(0, 255, 0, 255);
    this->draw_arrow(position_screen, arrow_screen, color, drawList);

}

void Scene2DManager::display_force(Object2D *object) {
    if (object == nullptr) return;
    display_forces_.insert(object);

}

void Scene2DManager::display_velocity(Object2D *object) {
    if (object == nullptr) return;
    display_velocity_.insert(object);

}

void Scene2DManager::hide_force(Object2D *object) {
    if (object == nullptr) return;
    display_forces_.erase(object);
}

void Scene2DManager::hide_velocity(Object2D *object) {
    if (object == nullptr) return;
    display_velocity_.erase(object);

}

Object2D *Scene2DManager::get_highlighted_object() {
    return highlighted_object_;
}

void Scene2DManager::energy_as_color(Object2D *object) {
    if (object == nullptr) return;
    float kinetic_energy = (object->get_velocity().Length()* object->get_velocity().Length()  * object->get_mass())/2;
    float potential_energy = object->get_position().y * object->get_mass() * current_scene_->get_world()->GetGravity().y;

    float total_energy = kinetic_energy + potential_energy;
    ImVec4 color = ImVec4(0, 0, 0, 1);

    color.x = (kinetic_energy / total_energy);
    color.y = 1 - color.x;
    //object->set_color(color);

}

void Scene2DManager::edit_object(b2Vec2 position, b2Vec2 size, float angle, float density, ImVec4 color) {
    if (highlighted_object_ != nullptr) {

        highlighted_object_->update_size(size);
        highlighted_object_->rotate(angle);
        highlighted_object_->set_density(density);
        highlighted_object_->set_color(color);
    }

}








