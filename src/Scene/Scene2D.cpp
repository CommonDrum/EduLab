//
// Created by Karol Hetman on 25/02/2023.
//

#include "Scene2D.h"

Scene2D::Scene2D(std::string name, b2Vec2 gravity) {
    m_name = name;
    m_camera = new Camera2D(0, 0, 0);
    m_world = new b2World(gravity);
}

Scene2D::~Scene2D() {
    delete m_camera;
    delete m_world;

}

void Scene2D::createBody(b2Vec2 position, b2Vec2 size, b2BodyType type, std::string shape) {
        //create a new box2d body
         b2BodyDef bodyDef;
         b2FixtureDef fixtureDef;
         bodyDef.type = type;
         bodyDef.position.Set(position.x, position.y);
         b2Body* body = m_world->CreateBody(&bodyDef);

    if(shape == "Box") {
        //create a box shape
        b2PolygonShape box;
        box.SetAsBox(size.x, size.y);
        //create a fixture
        fixtureDef.shape = &box;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        body->CreateFixture(&fixtureDef);
    }
    else if(shape == "Circle") {
            //create a circle shape
            b2CircleShape circle;
            circle.m_radius = size.x;
            fixtureDef.shape = &circle;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.3f;
            body->CreateFixture(&fixtureDef);}

    else if(shape == "Triangle") {
        b2PolygonShape triangle;
        triangle.SetAsBox(size.x, size.y);
        fixtureDef.shape = &triangle;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        body->CreateFixture(&fixtureDef);
    }
    else{
        std::cout << "Shape not found" << std::endl;
    }

}



std::string Scene2D::GetName() {
    return m_name;
}



/*---------------------------------------CAMERA-----------------------------------------*/

Camera2D::Camera2D(int32 posX, int32 posY, float zoom) {
    pos_x = posX;
    pos_y = posY;
    m_zoom = zoom;
}

void Camera2D::ResetView() {
    pos_x = 0;
    pos_y = 0;
    m_zoom = 1.0f;
}
