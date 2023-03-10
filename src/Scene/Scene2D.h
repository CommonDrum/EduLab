//
// Created by Karol Hetman on 25/02/2023.
//

#ifndef EDULAB_SCENE2D_H
#define EDULAB_SCENE2D_H
#include "core.h"
#include "Entity.h"

// Camera is a struct that will be applied to entities to get their position on the screen (in pixels)
struct Camera2D {
    Camera2D(int32 posX, int32 posY, float zoom);
    void ResetView();
    float m_zoom;
    int32 pos_x;
    int32 pos_y;
};

class Scene2D {
public:
    explicit Scene2D(std::string name,b2Vec2 gravity = b2Vec2(0.0f, -10.0f));
    ~Scene2D();
    std::string GetName();
    void createBody(b2Vec2 position, b2Vec2 size, b2BodyType type, std::string shape);

    void RemoveObject(int index);


    void SetCamera(Camera2D* camera);
    Camera2D* GetCamera();

private:
    std::string m_name;
    Camera2D* m_camera;
    b2World* m_world;
    std::vector<Entity*> m_entities;



};



#endif //EDULAB_SCENE2D_H
