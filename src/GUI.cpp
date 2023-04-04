//
// Created by Karol Hetman on 27/02/2023.
//

#include "GUI.h"
#include "imgui_internal.h"
#include "set"


// TODO: Devide UI into editor and game mode
// TODO: Add a way to create a new scene
// TODO: Display a list of scenes in the file explorer
// TODO: Display more information about the scene and highlighted object


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
    if (ImGui::Button("New Scene")) {
        ImGui::OpenPopup("Name Popup");
    }

    if (ImGui::BeginPopupModal("Name Popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char name[32] = "";
        ImGui::InputText("Name", name, IM_ARRAYSIZE(name));
        if (ImGui::Button("OK", ImVec2(120, 0))) {
            m_scene2DManager->create_scene(name);
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (ImGui::Button("Change Scene")) {
        ImGui::OpenPopup("File Popup");
    }

    if (ImGui::BeginPopupModal("File Popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        static std::set<std::string> filenames;
        const std::string directoryPath = "."; // Current directory
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            if (entry.is_regular_file()) {
                filenames.insert(entry.path().filename().string());
            }
        }

        static int selectedFileIndex = -1;
        int i = 0;
        for (const auto& filename : filenames) {
            const bool isSelected = (selectedFileIndex == i);
            if (ImGui::Selectable(filename.c_str(), isSelected)) {
                selectedFileIndex = i;
                const auto it = std::next(filenames.begin(), selectedFileIndex);
                const std::string selectedFilename = *it;
                m_scene2DManager->load_scene(selectedFilename);

            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
            i++;
        }

        if (ImGui::Button("OK", ImVec2(120, 0)) && selectedFileIndex >= 0) {
            const auto it = std::next(filenames.begin(), selectedFileIndex);
            const std::string selectedFilename = *it;
            m_scene2DManager->load_scene(selectedFilename);
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    ImGui::EndMainMenuBar();

}

void GUI::info() {
    ImGui::Begin("Information");


    static float width = 1.0f;
    static float height = 1.0f;
    static float rotation = 0.0f;
    if(m_scene2DManager->get_highlighted_object() != nullptr) {if (ImGui::Button("Show Force")){
            m_scene2DManager->get_highlighted_object()->set_show_forces(true);
        }
        if (ImGui::Button("Hide Force")){
            m_scene2DManager->get_highlighted_object()->set_show_forces(false);
        }
        if (ImGui::Button("Show Velocity")){
            m_scene2DManager->get_highlighted_object()->set_show_velocity(true);
        }
        if (ImGui::Button("Hide Velocity")){
            m_scene2DManager->get_highlighted_object()->set_show_velocity(false);
        }


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


    m_scene2DManager->draw_forces(draw_list, nullptr);
    m_scene2DManager->draw_velocity(draw_list);
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

void GUI::start(bool *done) {

}

void GUI::editor(bool *done) {
    menuBar(done);
    tools();
    info();
    mainViewport();


}

void GUI::play(bool *done) {
    mainViewport();

}

void GUI::main_menu(UI_State *pBoolean) {

    ImVec2 windowSize = ImVec2(400, 300);
    // create a window in the center of the current viewport
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - windowSize.x / 2, ImGui::GetIO().DisplaySize.y / 2 - windowSize.y / 2));
    ImGui::SetNextWindowSize(windowSize);

    ImGui::Begin("Main Menu", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

    ImGui::SetCursorPosY(ImGui::GetWindowSize().y / 2 - ImGui::GetTextLineHeightWithSpacing() - 10);
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 100);

    if (ImGui::Button("Play", ImVec2(200, 0))) {
        *pBoolean = UI_State::PLAY;
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetTextLineHeightWithSpacing() + 20);
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 100);

    if (ImGui::Button("Editor", ImVec2(200, 0))) {
        *pBoolean = UI_State::EDITOR;
    }

    ImGui::End();


}

void GUI::file_explorer(std::string *selectedFilename) {


    const std::string directoryPath = "/";
    std::vector<std::string> filenames;

// Iterate over files in directory and add their names to the vector
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            filenames.push_back(entry.path().filename().string());
        }
    }

// Sort filenames alphabetically
    std::sort(filenames.begin(), filenames.end());

// Create a combo box with the filenames
    if (ImGui::BeginCombo("File", selectedFilename->c_str())) {
        for (const auto& filename : filenames) {
            bool isSelected = (*selectedFilename == filename);
            if (ImGui::Selectable(filename.c_str(), isSelected)) {
                *selectedFilename = filename;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }


}


