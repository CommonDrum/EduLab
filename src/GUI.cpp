//
// Created by Karol Hetman on 27/02/2023.
//

#include "GUI.h"
#include "imgui_internal.h"






void GUI::menuBar(bool* done) {
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("EduLab")) {
        ImGui::Separator();
        if (ImGui::MenuItem("Exit", "Alt+F4" )) {
            *done = true;
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New", "Ctrl+N")) {
            //Open();
        }
        if (ImGui::MenuItem("Open", "Ctrl+O")) {
            //Open();
        }
        if (ImGui::MenuItem("Save", "Ctrl+S")) {
            //Save();
        }
        if (ImGui::MenuItem("Save As", "Ctrl+Shift+S")) {
            //SaveAs();
        }
        ImGui::EndMenu();
    }

    if(m_scene2DManager->running == false) {
        if (ImGui::Button("Play")) {
            m_scene2DManager->running = true;
        }
    }
    else {
        if (ImGui::Button("Stop")) {
            m_scene2DManager->running = false;
        }
       // ImGui::EndMenu();
    }








    ImGui::EndMainMenuBar();

}

void GUI::fileExplorer() {
    ImGui::Begin("File Explorer");

// Make sure you have a valid b2World pointer called 'world'
    ImGui::Text("Body Count: %zu", m_scene2DManager->get_current_scene()->get_bodies().size());

    ImGui::End();
        }

void GUI::tools() {
    ImGui::Begin("Tools");
    ImGui::Separator();
    ImGui::Begin("Body Creator");

    static float rotation = 0.0f;

    static float x = 0.0f;
    static float y = 0.0f;

    static float width = 1.0f;
    static float height = 1.0f;

    static float radius = 1.0f;

    const char* bodyTypes[] = { "Static", "Dynamic", "Kinematic" };
    static int bodyTypeIdx = 1; // Default to dynamic body

    const char* bodyShapes[] = { "Circle", "Rectangle" };
    static int bodyShapeIdx = 1; // Default to rectangle

    static ImVec4 boxColor(1.0f, 1.0f, 1.0f, 1.0f); // Default color (white)





    ImGui::Combo("Body Shape", &bodyShapeIdx, bodyShapes, IM_ARRAYSIZE(bodyShapes));
    ImGui::Combo("Body Type", &bodyTypeIdx, bodyTypes, IM_ARRAYSIZE(bodyTypes));

    ImGui::SliderFloat("X", &x, -1000.0f, 1000.0f);
    ImGui::SliderFloat("Y", &y, -1000.0f, 1000.0f);
    ImGui::SliderFloat("Rotation", &rotation, -180.0f, 180.0f);

    if (bodyShapeIdx == 0) { // Circle creation
        ImGui::InputFloat("Radius", &radius);
        ImGui::ColorEdit4("Color", (float *) &boxColor);
        if (ImGui::Button("Create Circle")) {
            auto bodyType = static_cast<b2BodyType>(bodyTypeIdx);
            m_scene2DManager->CreateCircle(x, y, radius, bodyType, boxColor, rotation);
        }
    }
    else if(bodyShapeIdx == 1) { // Rectangle creation
        ImGui::InputFloat("Width", &width);
        ImGui::InputFloat("Height", &height);
        ImGui::ColorEdit4("Color", (float*)&boxColor);
        if (ImGui::Button("Create Rectangle")) {
            auto bodyType = static_cast<b2BodyType>(bodyTypeIdx);
            m_scene2DManager->CreateBox(x, y, width, height, bodyType, boxColor, rotation);
            }
        }

    ImGui::End();
    ImGui::End();
    ///ImGui::SetWindowSize(ImVec2(300, ImGui::GetIO().DisplaySize.y));
}

void GUI::mainViewport() {
    ImGui::Begin("ImGui SDL Triangle");

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 curs_screen_pos = ImGui::GetCursorScreenPos();


    //create a box2d debug draw

    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 windowPos = ImGui::GetWindowPos();
    b2Vec2 center = b2Vec2(windowPos.x + windowSize.x / 2.0f, windowPos.y + windowSize.y / 2.0f);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    m_scene2DManager->draw_scene(draw_list, center);


    ImGui::End();
}



GUI::GUI(Scene2DManager* scene2DManager) {
    this->m_scene2DManager = scene2DManager;

}


