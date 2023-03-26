//
// Created by Karol Hetman on 04/03/2023.
//

#ifndef EDULAB_SCENE2DMANAGER_H
#define EDULAB_SCENE2DMANAGER_H
#include "Scene2D.h"
#include <cmath>



class Scene2DManager {
private:
    std::vector<Scene2D*> scenes_;
    Scene2D * current_scene_;
    Scene2D * original_scene_;
    bool m_running = true;
public:
    void create_scene(std::string name) ;

    void * CreateBody(b2BodyType bodyType, b2Vec2 position, b2Vec2 size, float angle = 0.0f, ImColor color = ImColor(255, 255, 255, 255));
    void * CreateBox(float x, float y, float width, float height, b2BodyType bodyType, ImVec4 color, float angle = 0.0f);
    b2Body * CreateCircle(float x, float y, float radius, b2BodyType bodyType, ImVec4 color, float angle);

    void DrawRectangle(const ImVec2 &position, const ImVec2 &size, float rotation, ImU32 color, ImDrawList* drawList);
    void DrawCircle(const ImVec2 &position, float radius, float rotation, ImU32 color);

    Object2D * object_at_point(b2Vec2 point);

    ImVec2 world_to_screen(const b2Vec2& worldCoords);
    b2Vec2 screen_to_world(const ImVec2& screenCoords);

    Scene2D* get_current_scene();
    void update();
    bool running = false;
    [[nodiscard]] std::vector<std::string> get_scene_names() const;


    void draw_scene(ImDrawList *draw_list);

    static void DrawFilledRectangle(const ImVec2 &position, const ImVec2 &size, float rotation, ImU32 color);


};



#endif //EDULAB_SCENE2DMANAGER_H
