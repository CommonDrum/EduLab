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
        void AddScene(Scene2D* scene);
        void RemoveScene(int index);
        void SetCurrentScene(Scene2D* index);
        void RenderCurrentScene();
        Scene2D* GetCurrentScene();
        void ClearScenes();
    private:
        std::vector<Scene2D*> m_scenes;
        Scene2D* m_currentScene;
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;

    };
};


#endif //EDULAB_SCENE2DMANAGER_H
