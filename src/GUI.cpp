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


    if(m_scene2DManager->running == false) {
        if (ImGui::Button("Play")) {
            m_scene2DManager->running = true;
        }
    }
    else {
        if (ImGui::Button("Stop")) {
            m_scene2DManager->running = false;
        }
    }
    if (ImGui::Button("Save")) {
        std::string name = m_scene2DManager->get_current_scene()->get_name();
        m_scene2DManager->save_scene(name);
    }
    if (ImGui::Button("Load")) {
        std::string name = m_scene2DManager->get_current_scene()->get_name();
        m_scene2DManager->load_scene(name);
    }
    ImGui::EndMainMenuBar();

}

void GUI::fileExplorer() {
    ImGui::Begin("File Explorer");


    static float width = 1.0f;
    static float height = 1.0f;
    static float rotation = 0.0f;

    ImGui::InputFloat("Width", &width);
    ImGui::InputFloat("Height", &height);
    ImGui::InputFloat("Rotation", &rotation);
    if (ImGui::Button("Change Rectangle")) {
    //m_scene2DManager->resize_highlighted_object(b2Vec2(width, height));
    m_scene2DManager->rotate_highlighted_object(rotation);
    }


    // Make sure you have a valid b2World pointer called 'world'

    ImGui::Text("Body Count: %zu", m_scene2DManager->get_current_scene()->get_objects().size());

    ImGui::End();
        }

void GUI::tools() {

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

    static float density = 1.0f;
    static float friction = 0.3f;
    static float restitution = 0.5f;


    static ImVec4 boxColor(1.0f, 1.0f, 1.0f, 1.0f); // Default color (white)



    ImGui::Combo("Body Shape", &bodyShapeIdx, bodyShapes, IM_ARRAYSIZE(bodyShapes));
    ImGui::Combo("Body Type", &bodyTypeIdx, bodyTypes, IM_ARRAYSIZE(bodyTypes));


    ImGui::InputFloat("X", &x);
    ImGui::InputFloat("Y", &y);
    ImGui::SliderFloat("Rotation", &rotation, -1.f, 1.f);
    ImGui::InputFloat("Density", &density);
    ImGui::InputFloat("Friction", &friction);
    ImGui::InputFloat("Bounce", &restitution);


    if (bodyShapeIdx == 0) { // Circle creation
        ImGui::InputFloat("Radius", &radius);
        ImGui::ColorEdit4("Color", (float *) &boxColor);
        if (ImGui::Button("Create Circle")) {
            auto bodyType = static_cast<b2BodyType>(bodyTypeIdx);
            m_scene2DManager->CreateCircle(x, y, radius, bodyType, boxColor, rotation, density, friction, restitution);
        }
    }
    else if(bodyShapeIdx == 1) { // Rectangle creation
        ImGui::InputFloat("Width", &width);
        ImGui::InputFloat("Height", &height);
        ImGui::ColorEdit4("Color", (float*)&boxColor);
        if (ImGui::Button("Create Rectangle")) {
            auto bodyType = static_cast<b2BodyType>(bodyTypeIdx);
            m_scene2DManager->CreateBox(x, y, width, height, bodyType, boxColor, rotation, density, friction, restitution);
            }
        }

    ImGui::End();

}

void GUI::mainViewport() {


    ImGui::Begin("ImGui SDL Triangle",NULL,ImGuiWindowFlags_NoScrollWithMouse);


    Camera * cam = m_scene2DManager->get_current_scene()->get_camera();

    ImVec2 center = windowCenter();

    cam->sUpdate(center.x, center.y); // Update the camera based on ImGui IO within the ImGui window




    // User Input
    if (ImGui::IsWindowHovered()) {
        float cameraSpeed = 10.0f;
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow))){
            cam->y += cameraSpeed;
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow))){
            cam->y -= cameraSpeed;
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_LeftArrow))){
            cam->x -= cameraSpeed;
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_RightArrow))){
            cam->x += cameraSpeed;
        }

        // Zoom Controls
        float zoomSpeed = 0.5f;
        cam->zoom += ImGui::GetIO().MouseWheel * zoomSpeed;
        if (cam->zoom < 0.1f) {
            cam->zoom = 0.1f;
        }

        // Body Movement
        if (ImGui::IsMouseClicked(0) && !m_scene2DManager->running) {
            b2Vec2 mousePos = m_scene2DManager->screen_to_world(ImGui::GetMousePos());
            m_scene2DManager->highlight_object_click(mousePos);
            clicked = true;
        }
        else if(ImGui::IsMouseClicked(0) && m_scene2DManager->running) {
            b2Vec2 mousePos = m_scene2DManager->screen_to_world(ImGui::GetMousePos());
            m_scene2DManager->attach_mouse_joint(mousePos);
            clicked = true;
        }

        if (clicked && ImGui::IsMouseDragging(0) &&  !m_scene2DManager->running) // Check if dragging
            {
                b2Vec2 mousePos = m_scene2DManager->screen_to_world(ImGui::GetMousePos());
                m_scene2DManager->move_highlighted_object(mousePos);
            }
        else if (clicked &&  ImGui::IsMouseDragging(0) && m_scene2DManager->running) // Check if dragging
            {
                b2Vec2 mousePos = m_scene2DManager->screen_to_world(ImGui::GetMousePos());
                m_scene2DManager->move_mouse_joint(mousePos);
                std::cout << "Dragging" << std::endl;
            }

        else if (ImGui::IsMouseReleased(0)) {
            clicked = false;
            m_scene2DManager->detach_mouse_joint();
        }
    }

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Draw ui of highlighted object


    m_scene2DManager->print_forces(draw_list);
    m_scene2DManager->draw_scene(draw_list);


    ImGui::End();
}



GUI::GUI(Scene2DManager* scene2DManager) {
    this->m_scene2DManager = scene2DManager;
    m_io.WantCaptureKeyboard = true;
    m_io.WantCaptureMouse = true;

}

ImVec2 GUI::windowCenter() {
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 center = ImVec2(windowPos.x + windowSize.x / 2.0f, windowPos.y + windowSize.y / 2.0f);
    return center;
}


