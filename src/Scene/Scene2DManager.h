//
// Created by Karol Hetman on 04/03/2023.
//

#ifndef EDULAB_SCENE2DMANAGER_H
#define EDULAB_SCENE2DMANAGER_H

#include "core.h"
#include "Scene2D.h"

class Scene2DManager {
// Scene should have os dependent code
// this is why scene2dManger will help us to manage windows and scenes
// it will be a singleton
    public:
        static Scene2DManager& GetInstance() {
            static Scene2DManager instance;
            return instance;
        }
        Scene2DManager();
        ~Scene2DManager();
        void CreateScene(std::string name);
        void RemoveScene(std::string name);
        void SetCurrentScene(std::string name);
        void RenderCurrentScene();
    private:
        std::vector<Scene2D*> m_scenes;
        Scene2D* m_currentScene;




};



#endif //EDULAB_SCENE2DMANAGER_H
