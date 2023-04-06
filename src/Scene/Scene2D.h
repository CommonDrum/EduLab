//
// Created by Karol Hetman on 15/03/2023.
//

#ifndef EDULAB_SCENE2D_H
#define EDULAB_SCENE2D_H

#include "core.h"





class Object2D {
private:
    b2Body* body_{};
    ImVec4 color;
    b2Vec2 size;
    bool showForces = false;
    bool showVelocity = false;
    std::string ID;
public:
    Object2D(b2Body* body, ImColor color, b2Vec2 size, std::string ID = "");

    // getter methods
    void set_show_forces(bool show) { showForces = show; }
    void set_show_velocity(bool show) { showVelocity = show; }

    bool is_showing_forces() { return showForces; }
    bool is_showing_velocity() { return showVelocity; }

    std::string get_id() { return ID; }
    std::string set_id(std::string newID) { ID = newID; return ID; }

    ImVec4 get_color() { return color; }
    ImVec4 set_color(ImVec4 newColor);

    void add_force(b2Vec2 force);

    float get_restitution() { return body_->GetFixtureList()->GetRestitution(); }
    void set_restitution(float restitution);

    b2Vec2 get_position();
    std::vector<b2Vec2> get_forces();
    b2Vec2 get_velocity();
    b2Body* get_body() { return body_; }

    float get_mass();
    float get_density();
    void set_density(float density);


    float get_friction(){ return body_->GetFixtureList()->GetFriction();}
    void set_friction(float friction);


    b2Vec2 get_size() { return size; }
    b2Vec2 update_size(b2Vec2 newSize) {  size = newSize; return size;}

    int get_shape();
    void set_size(b2Vec2 newSize)
    {
        // Copy all the necessary data
        b2BodyDef bodyDef;
        bodyDef.type = body_->GetType();
        bodyDef.position = body_->GetPosition();
        bodyDef.angle = body_->GetAngle();
        bodyDef.linearVelocity = body_->GetLinearVelocity();
        bodyDef.angularVelocity = body_->GetAngularVelocity();
        bodyDef.linearDamping = body_->GetLinearDamping();
        bodyDef.angularDamping = body_->GetAngularDamping();
        bodyDef.allowSleep = body_->IsSleepingAllowed();
        bodyDef.awake = body_->IsAwake();
        bodyDef.fixedRotation = body_->IsFixedRotation();
        bodyDef.bullet = body_->IsBullet();

        // Create a new body with the same properties depending on the shape
        if (this->get_shape() == 0 ){ //circle
            b2CircleShape circleShape;
            circleShape.m_radius = newSize.x;
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &circleShape;
            fixtureDef.density = body_->GetFixtureList()->GetDensity();
            fixtureDef.friction = body_->GetFixtureList()->GetFriction();
            fixtureDef.restitution = body_->GetFixtureList()->GetRestitution();
            b2Body* newBody = body_->GetWorld()->CreateBody(&bodyDef);
            newBody->CreateFixture(&fixtureDef);
            size = newSize;
            body_->GetWorld()->DestroyBody(body_);
            body_ = newBody;
        }
        else if (this->get_shape() == 2) { //rectangle
            b2PolygonShape polygonShape;
            polygonShape.SetAsBox(newSize.x, newSize.y);
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &polygonShape;
            fixtureDef.density = body_->GetFixtureList()->GetDensity();
            fixtureDef.friction = body_->GetFixtureList()->GetFriction();
            fixtureDef.restitution = body_->GetFixtureList()->GetRestitution();
            b2Body* newBody = body_->GetWorld()->CreateBody(&bodyDef);
            newBody->CreateFixture(&fixtureDef);
            size = newSize;
            body_->GetWorld()->DestroyBody(body_);
            body_ = newBody;
        }
    }


    float get_angle() { return body_->GetAngle(); }
    void set_angle(float angle) {
        body_->SetTransform(body_->GetPosition(), angle);
    }
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
    std::vector<Object2D*>& get_objects() { return objects_; }
    Camera* get_camera() { return &camera_; }
    [[nodiscard]] const std::string& get_name() const { return name_; }

    void MouseDown(const b2Vec2 &p);
    void MouseUp();
    void MouseMove(const b2Vec2 &p);

    Object2D * add_object(b2Body* body, ImVec4 color, b2Vec2 size);
    void delete_object(b2Body* body);


    void serialize(std::string filename);
    void deserialize(std::string filename);



private:
    b2World* world_;
    std::vector<Object2D*> objects_;
    Camera camera_ = Camera(0, 0, 10, 0, 0);
    b2Body* groundBody_;
    b2MouseJoint* mouseJoint_ = NULL;
    b2Vec2 mouseWorld_;

    std::string name_;


    Object2D *CreateBox(float x, float y, float width, float height, b2BodyType bodyType, ImVec4 color, float angle,
                        float density,
                        float friction, float restitution, b2Vec2 velocity = b2Vec2(0, 0));

    Object2D *
    CreateCircle(float x, float y, float radius, b2BodyType bodyType, ImVec4 color, float angle, float density,
                 float friction, float restitution, b2Vec2 velocity = b2Vec2(0, 0));
};



#endif //EDULAB_SCENE2D_H
