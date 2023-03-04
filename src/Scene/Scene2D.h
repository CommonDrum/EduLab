//
// Created by Karol Hetman on 25/02/2023.
//

#ifndef EDULAB_SCENE2D_H
#define EDULAB_SCENE2D_H
#include "core.h"
#include "Entity.h"

//Scene2D
//It contains a camera, a list of objects
//It is responsible for rendering the scene to a window and updating the objects

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
    // Create the window and start playing the scene
    void CreateContext(SDL_Window *window);
    void DestroyContext();

    void Render();
    void Resize(int32 width, int32 height);


    void AddObject(b2Vec2 position, b2Vec2 size);
    void RemoveObject(int index);

    SDL_Window * GetWindow();


    void ClearObjects();
    void SetCamera(Camera2D* camera);
    Camera2D* GetCamera();

private:
    int32 m_WindowWidth;
    int32 m_WindowHeight;

    Camera2D* m_camera;
    std::vector<Entity*> m_objects;
    b2World* m_worldLocal;
    std::vector<SDL_Vertex> m_vertices;
    std::vector<int> m_indices;
    SDL_Window* m_window{};
    SDL_Renderer* m_renderer{};
    SDL_Event m_event;


};


#endif //EDULAB_SCENE2D_H
