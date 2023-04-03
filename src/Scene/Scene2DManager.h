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
    Object2D * highlighted_object_ = nullptr;
    bool m_running = false;
public:

    void create_scene(std::string name) ;
    void reset_scene();
    void save_scene(std::string name);
    void load_scene(std::string name);


    void *CreateBox(float x, float y, float width, float height, b2BodyType bodyType, ImVec4 color, float angle = 0.0f,
                    float density = 1.0f, float friction = 0.3f, float restitution = 0.1f);
    b2Body * CreateCircle(float x, float y, float radius, b2BodyType bodyType, ImVec4 color, float angle = 0.0f,
                          float density = 1.0f, float friction = 0.3f, float restitution = 0.1f);

    void DrawRectangle(const ImVec2 &position, const ImVec2 &size, float rotation, ImU32 color, ImDrawList* drawList);
    void DrawCircle(const ImVec2 &position, float radius, float rotation, ImU32 color);
    void draw_arrow(const ImVec2 &position, const ImVec2 &size, ImU32 color, ImDrawList* drawList);

    Object2D * object_at_point(b2Vec2 point);
    void highlight_object_click(b2Vec2 point);
    void move_highlighted_object(b2Vec2 point);
    json get_highlighted_stats();
    void draw_forces(ImDrawList *drawList);
    void draw_velocity(ImDrawList *drawList, Object2D *object);

    void attach_mouse_joint(b2Vec2 point);
    void detach_mouse_joint();
    void move_mouse_joint(b2Vec2 point);

    void resize_highlighted_object(b2Vec2 newSize);
    void rotate_highlighted_object(float angle);

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
