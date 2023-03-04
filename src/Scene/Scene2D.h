//
// Created by Karol Hetman on 25/02/2023.
//

#ifndef EDULAB_SCENE2D_H
#define EDULAB_SCENE2D_H
#include "core.h"
#include "Entity.h"

// Camera is a struct that will be applied to entities to get their position on the screen (in pixels)
struct Camera2D {
    explicit Camera2D(int32 width = DEFAULT_WIDTH, int32 height = DEFAULT_HEIGHT, b2Vec2 center = b2Vec2(0.0f, 0.0f), float zoom = 1.0f);
    void ResetView();
    float m_zoom;
    b2Vec2 m_center;
    int32 m_width;
    int32 m_height;
};

class Scene2D {
public:
    explicit Scene2D(int32 width = DEFAULT_WIDTH,int32 high = DEFAULT_HEIGHT ,b2Vec2 gravity = b2Vec2(0.0f, -10.0f));
    ~Scene2D();

    std::vector<Entity*> GetObjects(); // Get all objects in the scene

    void AddObject(b2Vec2 position, b2Vec2 size);
    void RemoveObject(int index);


    void SetCamera(Camera2D* camera);
    Camera2D* GetCamera();

private:

    Camera2D* m_camera;
    std::vector<Entity*> m_objects;
    b2World* m_worldLocal;
    int m_windowWidth{};
    int m_windowHeight{};


};



#endif //EDULAB_SCENE2D_H
