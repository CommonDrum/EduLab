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

    if (ImGui::BeginMenu("Scenes")) {

        std::vector<std::string> strings = { "apple", "banana", "cherry", "date" };
        int selected_index = 0;

        if (ImGui::BeginCombo("##combo", strings[selected_index].c_str())) {
            for (int i = 0; i < strings.size(); ++i) {
                const bool is_selected = (selected_index == i);
                if (ImGui::Selectable(strings[i].c_str(), is_selected))
                    selected_index = i;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::EndMenu();
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

    static float x = 0.0f;
    static float y = 0.0f;
    static float width = 1.0f;
    static float height = 1.0f;
    static int bodyTypeIdx = 1; // Default to dynamic body
    static ImVec4 boxColor(1.0f, 0.0f, 0.0f, 1.0f); // Default color (red)


    ImGui::InputFloat("X", &x);
    ImGui::InputFloat("Y", &y);
    ImGui::InputFloat("Width", &width);
    ImGui::InputFloat("Height", &height);

    const char* bodyTypes[] = { "Static", "Dynamic", "Kinematic" };
    ImGui::Combo("Body Type", &bodyTypeIdx, bodyTypes, IM_ARRAYSIZE(bodyTypes));

    ImGui::ColorEdit4("Color", (float*)&boxColor);

    if (ImGui::Button("Create Box")) {
        b2BodyType bodyType = static_cast<b2BodyType>(bodyTypeIdx);
        m_scene2DManager->CreateBox( x, y, width, height, bodyType, boxColor);
        // You may want to store the created body (newBox) in a data structure, like a vector, for future reference.
    }
    ImGui::SetWindowSize(ImVec2(300, ImGui::GetIO().DisplaySize.y));
    ImGui::Text("Tools");
    ImGui::Separator();
    ImGui::End();
}

void GUI::mainViewport() {
    ImGui::Begin("ImGui SDL Triangle");

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 screen_pos = ImGui::GetCursorScreenPos();
    float size = 100.0f;
    float x = pos.x + ImGui::GetWindowWidth() / 2.0f - size / 2.0f;
    float y = pos.y + ImGui::GetWindowHeight() / 2.0f - size / 2.0f;
    std::cout << x << " " << y << std::endl;

    //create a box2d debug draw

        ImDrawList* draw_list = ImGui::GetWindowDrawList();

      m_scene2DManager->draw_scene(draw_list);


    ImGui::End();
}

void GUI::placeholder(SDL_Window* sdl_window){
    ImGui::Begin("Imgui Window");

// Create a placeholder for the SDL window
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 size = ImVec2(320, 240);
    ImGui::GetWindowDrawList()->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32_WHITE);

// Dock the SDL window into the placeholder
    ImGuiID sdl_window_id = ImGui::GetWindowViewport()->ID;
    //ImGui::DockBuilderDockWindow(reinterpret_cast<const char *>(SDL_GetWindowID(sdl_window)), sdl_window_id);
    ImGui::End();
}

GUI::GUI(Scene2DManager* scene2DManager) {
    this->m_scene2DManager = scene2DManager;

}


