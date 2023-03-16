//
// Created by Karol Hetman on 15/03/2023.
//

#ifndef EDULAB_SCENE2D_H
#define EDULAB_SCENE2D_H

#include "core.h"


struct Camera {
    b2Vec2 position;
    float zoom;
    b2Vec2 screen;
};

class Scene2D {
public:
    explicit Scene2D(std::string name);

    // getter methods
    b2World* get_world() { return world_; }
    std::vector<std::pair<b2Body*, ImVec4>>& get_bodies() { return bodies_; }
    [[nodiscard]] const Camera& get_camera() const { return camera_; }
    [[nodiscard]] const std::string& get_name() const { return name_; }

private:
    b2World* world_;
    std::vector<std::pair<b2Body*, ImVec4>> bodies_;
    Camera camera_ = {b2Vec2(0, 0), 1, b2Vec2(0, 0)};
    std::string name_;
};



#endif //EDULAB_SCENE2D_H
