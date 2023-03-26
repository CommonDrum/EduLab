//
// Created by Karol Hetman on 15/03/2023.
//

#ifndef EDULAB_SCENE2D_H
#define EDULAB_SCENE2D_H

#include "core.h"

class Object2D {
private:
    b2Body* body{};
    ImVec4 color;
public:
    Object2D(b2Body* body, ImColor color);

    // getter methods
    b2Body* get_body() { return body; }
    ImVec4 get_color() { return color; }
    ImVec4 set_color(ImVec4 newColor);
};




struct Camera {
    float x, y; // Position of the camera
    float zoom; // Zoom level of the camera
    float sX, sY; // Screen size
    void sUpdate(float newX, float newY) { // Screen update
        this->sX = newX;
        this->sY = newY;
    }

    // Constructor to initialize the camera's position and zoom
    Camera(float x, float y, float zoom, float sX = 0, float sY = 0)
            : x(x), y(y), zoom(zoom), sX(sX), sY(sY) {}


    // Function to print the camera's position and zoom level
    void printInfo() const {
        std::cout << "Camera position: (" << x << ", " << y << ")\n";
        std::cout << "Camera zoom level: " << zoom << std::endl;
    }
};

class Scene2D {
public:
    explicit Scene2D(std::string name);

    // getter methods
    b2World* get_world() { return world_; }
    std::vector<std::pair<b2Body*, ImVec4>>& get_bodies() { return bodies_; }
    std::vector<Object2D*>& get_objects() { return objects_; }
    Camera* get_camera() { return &camera_; }
    [[nodiscard]] const std::string& get_name() const { return name_; }

    void add_object(b2Body* body, ImVec4 color);
    void delete_object(b2Body* body);

private:
    b2World* world_;
    std::vector<std::pair<b2Body*, ImVec4>> bodies_;
    std::vector<Object2D*> objects_;
    Camera camera_ = Camera(0, 0, 10, 0, 0);
    std::string name_;
};



#endif //EDULAB_SCENE2D_H
