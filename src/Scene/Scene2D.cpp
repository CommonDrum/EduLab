//
// Created by Karol Hetman on 15/03/2023.
//

#include "Scene2D.h"



class QueryCallback : public b2QueryCallback
{
public:
    QueryCallback(const b2Vec2& point)
    {
        m_point = point;
        m_fixture = NULL;
    }

    bool ReportFixture(b2Fixture* fixture) override
    {
        b2Body* body = fixture->GetBody();
        if (body->GetType() == b2_dynamicBody)
        {
            bool inside = fixture->TestPoint(m_point);
            if (inside)
            {
                m_fixture = fixture;

                // We are done, terminate the query.
                return false;
            }
        }

        // Continue the query.
        return true;
    }

    b2Vec2 m_point;
    b2Fixture* m_fixture;
};

Scene2D::Scene2D(std::string name) {
    world_ = new b2World(b2Vec2(0, 9.81f));
    name_ = std::move(name);
    b2BodyDef bodyDef;
    groundBody_ = world_->CreateBody(&bodyDef);


}

void Scene2D::add_object(b2Body *body, ImVec4 color, b2Vec2 size) {
    auto* object = new Object2D(body, color, size);


    objects_.push_back(object);
}

void Scene2D::delete_object(b2Body *body) {
    for (auto it = objects_.begin(); it != objects_.end(); ++it) {
        if ((*it)->get_body() == body) {
            world_->DestroyBody(body);
            objects_.erase(it);
            break;
        }
    }

}
void Scene2D::MouseDown(const b2Vec2& p) { // Copied from Box2D example
    mouseWorld_ = p;

    if (mouseJoint_ != NULL)
    {
        return;
    }

    // Make a small box.
    b2AABB aabb{};
    b2Vec2 d;
    d.Set(0.001f, 0.001f);
    aabb.lowerBound = p - d;
    aabb.upperBound = p + d;

    // Query the world for overlapping shapes.
    QueryCallback callback(p);
    world_->QueryAABB(&callback, aabb);

    if (callback.m_fixture)
    {
        float frequencyHz = 5.0f;
        float dampingRatio = 0.7f;

        b2Body* body = callback.m_fixture->GetBody();
        b2MouseJointDef jd;
        jd.bodyA = groundBody_;
        jd.bodyB = body;
        jd.target = p;
        jd.maxForce = 1000.0f * body->GetMass();
        b2LinearStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);

        mouseJoint_ = (b2MouseJoint*)world_->CreateJoint(&jd);
        body->SetAwake(true);
    }
}

void Scene2D::MouseMove(const b2Vec2& p) // Copied from Box2D example
{
    mouseWorld_ = p;

    if (mouseJoint_)
    {
        mouseJoint_->SetTarget(p);
    }
}

void Scene2D::MouseUp() {
    if (mouseJoint_)
    {
        world_->DestroyJoint(mouseJoint_);
        mouseJoint_ = NULL;
    }
}

// TODO: ADD VELOCITY TO DESERIALIZER
void Scene2D::serialize(std::string filename) {
    json j;
    j["name"] = name_;
    j["gravity"] = {world_->GetGravity().x, world_->GetGravity().y};
    for (auto object : objects_) {
        json object_data;
        object_data["size"]= {object->get_size().x, object->get_size().y};
        object_data["position"] = {object->get_body()->GetPosition().x, object->get_body()->GetPosition().y};
        object_data["angle"] = object->get_body()->GetAngle();
        object_data["linear_velocity"] = {object->get_body()->GetLinearVelocity().x, object->get_body()->GetLinearVelocity().y};
        object_data["angular_velocity"] = object->get_body()->GetAngularVelocity();
        object_data["linear_damping"] = object->get_body()->GetLinearDamping();
        object_data["angular_damping"] = object->get_body()->GetAngularDamping();

        object_data["type"] = object->get_body()->GetType();
        object_data["shape"] = object->get_body()->GetFixtureList()->GetType();
        object_data["density"] = object->get_body()->GetFixtureList()->GetDensity();
        object_data["friction"] = object->get_body()->GetFixtureList()->GetFriction();
        object_data["restitution"] = object->get_body()->GetFixtureList()->GetRestitution();
        object_data["color"] = {object->get_color().x, object->get_color().y, object->get_color().z, object->get_color().w};
        object_data["showForces"] = object->is_showing_forces();
        object_data["showVelocity"] = object->is_showing_velocity();
        j["objects"].push_back(object_data);
}
    // Save serialized data to file
    std::ofstream file(filename);
    file << j.dump(4);

}

void Scene2D::deserialize(std::string filename) {
    // Clear existing data
    for (int i = 0; i < objects_.size(); i++) {
        Object2D* object = objects_[i];
        world_->DestroyBody(object->get_body());
    }
    objects_.clear();


    // Load serialized data from file
    std::ifstream file(filename);
    json j;
    file >> j;

    // Set scene name
    name_ = j["name"];

    // Set gravity
    world_->SetGravity(b2Vec2(j["gravity"][0], j["gravity"][1]));



    // Create objects
    for (auto object : j["objects"]) {
        ImVec4 color = {object["color"][0], object["color"][1], object["color"][2], object["color"][3]};

        if (object["shape"] == 2) {
          CreateBox(object["position"][0], object["position"][1], object["size"][0], object["size"][1],
                      (b2BodyType) object["type"], color, object["angle"],
                      object["density"], object["friction"], object["restitution"]);

        }
        else if(object["shape"] == 0){
            CreateCircle(object["position"][0], object["position"][1], object["size"][0],
                      (b2BodyType) object["type"], color, object["angle"],
                      object["density"], object["friction"], object["restitution"]);
        }
        objects_.back()->set_show_forces(object["showForces"]);
        objects_.back()->set_show_velocity(object["showVelocity"]);
    }

    // close file
    file.close();


}

void * Scene2D::CreateBox(float x, float y, float width, float height, b2BodyType bodyType, ImVec4 color,
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

    b2Body* body = world_->CreateBody(&bodyDef);
    body->CreateFixture(&fixtureDef);

    add_object(body, color, b2Vec2(width, height));

    // return the pair
    return body;
}

b2Body * Scene2D::CreateCircle(float x, float y, float radius, b2BodyType bodyType, ImVec4 color, float angle,
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

    b2Body* body = world_->CreateBody(&bodyDef);
    body->CreateFixture(&fixtureDef);

    add_object(body, color, b2Vec2(radius, radius));

    return body;
}

Object2D::Object2D(b2Body *body, ImColor color, b2Vec2 size) { // because i will probably need body to store more inforamtion  like forces etc
    this->body_ = body;
    this->color = color;
    this->size = size;

}

ImVec4 Object2D::set_color(ImVec4 newColor) {
    color = newColor;
    return color;

}



std::vector<b2Vec2> Object2D::get_forces() {


    std::vector<b2Vec2> forces;
    forces.push_back( body_->GetMass() * body_->GetWorld()->GetGravity());




    b2ContactEdge* contactEdge = body_->GetContactList();
    while (contactEdge != nullptr) {
        // Access the contact information
        b2Contact *contact = contactEdge->contact;

        // Get the contact force/*
        b2Vec2 contactForce(0.0f, 0.0f);
        if (contact->IsTouching()) {
            b2WorldManifold worldManifold;
            contact->GetWorldManifold(&worldManifold);

            for (int i = 0; i < contact->GetManifold()->pointCount; ++i) {
                contactForce += contact->GetManifold()->points[i].normalImpulse * worldManifold.normal;
            }
        }

        forces.push_back(contactForce);
        // Move to the next contact edge
        contactEdge = contactEdge->next;
    }

    return forces;
}

b2Vec2 Object2D::get_velocity() {

    return this->body_->GetLinearVelocity();
}

void Object2D::add_force(b2Vec2 force) {
    this->body_->ApplyForceToCenter(force, true);

}

void Object2D::set_density(float density) {
    this->body_->GetFixtureList()->SetDensity(density);
    this->body_->ResetMassData();

}

float Object2D::get_mass() {
    return this->body_->GetMass();
}

b2Vec2 Object2D::get_position() {
    return this->body_->GetPosition();
}

void Object2D::set_restitution(float restitution) {
    this->body_->GetFixtureList()->SetRestitution(restitution);

}

float Object2D::get_density() {
    return body_->GetFixtureList()->GetDensity();
}

int Object2D::get_shape() {
    return body_->GetFixtureList()->GetShape()->GetType();
}

void Object2D::set_friction(float friction) {
    this->body_->GetFixtureList()->SetFriction(friction);

}



