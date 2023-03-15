//
// Created by Karol Hetman on 04/03/2023.
//

#ifndef EDULAB_SCENE2DMANAGER_H
#define EDULAB_SCENE2DMANAGER_H
#include "Scene2D.h"
#include <utility>

#include "core.h"


class Scene2DManager {
private:
    std::vector<Scene2D*> scenes_;
    Scene2D * current_scene_;
public:
    void create_scene(std::string name) ;

    [[nodiscard]] std::vector<std::string> get_scene_names() const;





};



#endif //EDULAB_SCENE2DMANAGER_H
