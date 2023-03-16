//
// Created by Karol Hetman on 04/03/2023.
//

#ifndef EDULAB_SCENE2DMANAGER_H
#define EDULAB_SCENE2DMANAGER_H
#include "Scene2D.h"



class Scene2DManager {
private:
    std::vector<Scene2D*> scenes_;
    Scene2D * current_scene_;
public:
    void create_scene(std::string name) ;
    void * CreateBox(float x, float y, float width, float height, b2BodyType bodyType, ImVec4 color);
    void draw_scene(ImDrawList *draw_list);

    [[nodiscard]] std::vector<std::string> get_scene_names() const;


    b2Body *CreateCircle(float x, float y, float radius, b2BodyType bodyType, ImVec4 color);
};



#endif //EDULAB_SCENE2DMANAGER_H
