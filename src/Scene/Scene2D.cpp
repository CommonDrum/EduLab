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

void Scene2D::add_object(b2Body *body, ImVec4 color) {
    auto* object = new Object2D(body, color);
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
        mouseJoint_ = nullptr;
    }
}

Object2D::Object2D(b2Body *body, ImColor color) { // because i will probably need body to store more inforamtion  like forces etc
    this->body = body;
    this->color = color;

}

ImVec4 Object2D::set_color(ImVec4 newColor) {
    color = newColor;
    return color;

}



