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
        if(ImGui::MenuItem("Show/Hide Grid", "G")) {
            show_grid = !show_grid;
        }
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

    std::string name = m_scene2DManager->get_current_scene()->get_name();
    ImGui::Text("Scene Name: %s", name.c_str());
    std::vector<std::string> editor_options = { "rotation", "color", "density", "friction", "restitution","vectors","size","gravity"};
    if (state == UI_State::PLAY){
        editor_options = *m_scene2DManager->get_editable_properties();
    }
    this->object_editor(&editor_options);
    ImGui::SameLine();


    // Make sure you have a valid b2World pointer called 'world'

    ImGui::Text("Body Count: %zu", m_scene2DManager->get_current_scene()->get_objects().size());

    ImGui::End();
        }

void GUI::tools() {
    joint_creation();
    force_application();

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


    ImGui::Begin("Main Viewport",NULL,ImGuiWindowFlags_NoScrollWithMouse| ImGuiWindowFlags_NoTitleBar);







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

        // Drag Navigation
        b2Vec2 mousePos = m_scene2DManager->screen_to_world(ImGui::GetMousePos());
        if (ImGui::IsMouseDragging(1)) {
            cam->x -= ImGui::GetIO().MouseDelta.x;
            cam->y += ImGui::GetIO().MouseDelta.y;
        }


        b2Vec2  object_pos(0,0);
        if (ImGui::IsMouseDoubleClicked(0)&& m_scene2DManager->object_at_point(mousePos) != nullptr) {
            this->show_object_properties = true;
            object_pos = m_scene2DManager->object_at_point(mousePos)->get_body()->GetPosition();
        }
        if (ImGui::IsMouseDoubleClicked(0)&& m_scene2DManager->object_at_point(mousePos) == nullptr) {
            this->show_object_properties = false;
            object_pos = b2Vec2(0,0);
        }

        if(this->show_object_properties) {
            object_properties_popup(m_scene2DManager->world_to_screen(object_pos));
        }

        if (ImGui::IsMouseDragging(0) && enable_force_application){
            mousePos = m_scene2DManager->screen_to_world(ImGui::GetMousePos());
            m_scene2DManager->draw_arrow(ImGui::GetMousePos(),ImGui::GetMouseDragDelta(0),ImU32{},ImGui::GetWindowDrawList());
            if(ImGui::IsMouseReleased(0)){
                m_scene2DManager->apply_force(mousePos,m_scene2DManager->screen_to_world(ImGui::GetMouseDragDelta(0)));
            }
        }

        else if (ImGui::IsMouseClicked(0) && !m_scene2DManager->running) {
            m_scene2DManager->highlight_object_click(mousePos);
            clicked = true;
        }
        else if(ImGui::IsMouseClicked(0) && m_scene2DManager->running) {
            m_scene2DManager->highlight_object_click(mousePos);
            m_scene2DManager->attach_mouse_joint(mousePos);
            clicked = true;
        }

        if (clicked && ImGui::IsMouseDragging(0) &&  !m_scene2DManager->running) // Check if dragging
            {
                m_scene2DManager->move_highlighted_object(mousePos);
            }
        else if (clicked &&  ImGui::IsMouseDragging(0) && m_scene2DManager->running) // Check if dragging
            {
                m_scene2DManager->move_mouse_joint(mousePos);
            }




        else if (ImGui::IsMouseReleased(0)) {
            clicked = false;
            m_scene2DManager->detach_mouse_joint();
        }


        if (joint_creation_enabled && ImGui::IsMouseClicked(0) && clicks == 0){
            clicks++;
            pos1 = ImGui::GetMousePos();
        }
        else if(clicks == 1 && joint_creation_enabled && ImGui::IsMouseClicked(0)){
            clicks = 0;
            pos2 = ImGui::GetMousePos();
            m_scene2DManager->connect(pos1, pos2,joint_creation_type);
            std::cout << "Joint Created" << std::endl;
        }
    }

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Draw grid inside the viewport

    if (show_grid){
        m_scene2DManager->draw_grid(draw_list);
    }




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
    // Choose a scene to play
    if (state == UI_State::MAIN_MENU){
        ImGui::OpenPopup("File Popup");
        m_scene2DManager->running = true;
        state = UI_State::PLAY;

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

    mainViewport();
    info();

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

void GUI::object_editor(std::vector<std::string> *options) {

    // Editor interface for editing the highlighted object
    // Displayed options are determined by the options vector


    if(m_scene2DManager->get_highlighted_object() != nullptr) {


        bool option_size = false;
        bool option_density = false;
        bool option_gravity = false;
        bool option_rotation = false;
        bool option_restitution = false;
        bool option_color = false;
        bool option_delete = false;
        bool option_vectors = false;
        bool option_friction = false;

        for (auto &option : *options) {
            if (option == "size") {
                option_size = true;
            }
            if (option == "density") {
                option_density = true;
            }
            if (option == "gravity") {
                option_gravity = true;
            }
            if (option == "rotation") {
                option_rotation = true;
            }
            if (option == "restitution") {
                option_restitution = true;
            }
            if (option == "color") {
                option_color = true;
            }
            if (option == "delete") {
                option_delete = true;
            }
            if (option == "vectors") {
                option_vectors = true;
            }
            if (option == "friction") {
                option_friction = true;
            }
        }

        b2Vec2 size = m_scene2DManager->get_highlighted_object()->get_size();
        float density = m_scene2DManager->get_highlighted_object()->get_density();
        float gravity = m_scene2DManager->get_current_scene()->get_world()->GetGravity().y;
        int rotation = static_cast<int>(m_scene2DManager->get_highlighted_object()->get_angle() * 180.0f / b2_pi);
        int stepSize = 5; // Set the desired step size in degrees
        int scaledAngle = rotation / stepSize;
        float restitution = m_scene2DManager->get_highlighted_object()->get_restitution();
        float friction = m_scene2DManager->get_highlighted_object()->get_friction();
        ImVec4 color = m_scene2DManager->get_highlighted_object()->get_color();

        if (option_rotation){
            ImGui::SetNextItemWidth(100);
            if(ImGui::SliderInt("Rotation", &scaledAngle, 0, 360 / stepSize, "%.1f")){
                rotation = scaledAngle * stepSize;
                m_scene2DManager->get_highlighted_object()->set_angle((float)rotation* b2_pi / 180.0f);
            }
        }
        if (option_color){
            ImGui::SameLine();
            if(ImGui::ColorEdit4("Color", (float *) &color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel)){
                m_scene2DManager->get_highlighted_object()->set_color(color);
            }
        }

        if (option_restitution){
            ImGui::SetNextItemWidth(100);
            if(ImGui::SliderFloat("Restitution", &restitution, -1.0f, 8.0f, "%.1f", 0.5f)){
                m_scene2DManager->get_highlighted_object()->set_restitution(restitution);
            }
        }

        if (option_size){
            if (m_scene2DManager->get_highlighted_object()->get_shape() == 2){
                ImGui::SetNextItemWidth(100);
                if(ImGui::SliderFloat("Width", &size.x, 0.0f, 100.0f, "%.1f", 0.5f)){
                    m_scene2DManager->get_highlighted_object()->set_size(size);
                }
                ImGui::SameLine();
                ImGui::SetNextItemWidth(100);
                if(ImGui::SliderFloat("Height", &size.y, 0.0f, 100.0f, "%.1f", 0.5f)){
                    m_scene2DManager->get_highlighted_object()->set_size(size);
                }
            }
            if (m_scene2DManager->get_highlighted_object()->get_shape() == 0){
                ImGui::SetNextItemWidth(100);
                if(ImGui::SliderFloat("Radius", &size.x, 0.0f, 100.0f, "%.1f", 0.5f)){
                    m_scene2DManager->get_highlighted_object()->set_size(size);
                }
            }
        }

        if(option_density){
            ImGui::SetNextItemWidth(100);
            if(ImGui::SliderFloat("Density", &density, 0.0f, 10.0f, "%.1f", 0.5f)){
                m_scene2DManager->get_highlighted_object()->set_density(density);
            }
        }
        if (option_friction){
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100);
            if(ImGui::SliderFloat("Friction", &friction, 0.0f, 10.0f, "%.1f", 1.0f)){
                m_scene2DManager->get_highlighted_object()->set_friction(density);
            }
        }

        if(option_gravity){
            ImGui::SetNextItemWidth(100);
            if(ImGui::SliderFloat("Gravity", &gravity, -10.0f, 10.0f, "%.1f", 1.0f)){
                m_scene2DManager->get_current_scene()->get_world()->SetGravity(b2Vec2(0.0f, gravity));
            }
        }


        if(option_vectors){
            ImGui::SetNextItemWidth(50);
            if (ImGui::Button("Show Force")){
                m_scene2DManager->get_highlighted_object()->set_show_forces(true);
            }
            ImGui::SameLine();
            ImGui::SetNextItemWidth(50);
            if (ImGui::Button("Hide Force")){
                m_scene2DManager->get_highlighted_object()->set_show_forces(false);
            }
            ImGui::SetNextItemWidth(50);
            if (ImGui::Button("Show Velocity")){
                m_scene2DManager->get_highlighted_object()->set_show_velocity(true);
            }
            ImGui::SetNextItemWidth(50);
            if (ImGui::Button("Hide Velocity")){
                m_scene2DManager->get_highlighted_object()->set_show_velocity(false);
            }
        }
    }
}

void GUI::object_properties_popup(ImVec2 pos) {
    if (m_scene2DManager->get_highlighted_object() == nullptr) return;
    ImGui::SetNextWindowPos(pos);
    if (ImGui::Begin("Object Properties",NULL,ImGuiWindowFlags_NoTitleBar| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_NoNav)) {
        ImGui::Text("Position: (%.1f, %.1f)", m_scene2DManager->get_highlighted_object()->get_position().x, m_scene2DManager->get_highlighted_object()->get_position().y);
        ImGui::Text("Velocity: (%.1f, %.1f)", m_scene2DManager->get_highlighted_object()->get_velocity().x, m_scene2DManager->get_highlighted_object()->get_velocity().y);
        ImGui::Text("Mass: %.1f", m_scene2DManager->get_highlighted_object()->get_mass());
        ImGui::End();
    }

}

void GUI::joint_creation() {
    ImGui::Begin("Joint Creation");
    if (joint_creation_enabled){
        if(ImGui::Button("Disable Joint Creation")){
            joint_creation_enabled = false;
    }
    } else {
        if(ImGui::Button("Enable Joint Creation")){
            joint_creation_enabled = true;
        }
    }

    if(ImGui::Button("Wheel Joint")){
        joint_creation_type = 6;
    }
    if(ImGui::Button("Rev Joint")){
        joint_creation_type = 1;
    }
    ImGui::End();
}

void GUI::force_application() {
    ImGui::Begin("Forces");
    if(ImGui::Button("Enable/Disable force application")){
        enable_force_application = !enable_force_application;
    }
    ImGui::End();


}


