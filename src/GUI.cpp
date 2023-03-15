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
    //Create window with icons and names of files in directory

    ImGui::End();
        }

void GUI::tools() {
    ImGui::Begin("Tools");
    //Start the window on the right side of the screen
    ImGui::SetWindowSize(ImVec2(300, ImGui::GetIO().DisplaySize.y));
    ImGui::Text("Tools");
    ImGui::Separator();
    ImGui::End();
}

void GUI::mainViewport(b2World* world) {
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
        for (b2Body* body = world->GetBodyList(); body != nullptr; body = body->GetNext())
        {
            for (b2Fixture* fixture = body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
            {
                b2Shape* shape = fixture->GetShape();
                if (shape->GetType() == b2Shape::e_circle)
                {
                    auto* circle_shape = dynamic_cast<b2CircleShape*>(shape);
                    b2Vec2 center = body->GetWorldPoint(circle_shape->m_p);
                    float radius = circle_shape->m_radius;
                    draw_list->AddCircle(ImVec2(center.x, center.y), radius, IM_COL32(255, 0, 0, 255));
                }
                else if (shape->GetType() == b2Shape::e_polygon)
                {
                    auto* polygon_shape = dynamic_cast<b2PolygonShape*>(shape);
                    int num_vertices = polygon_shape->m_count;
                    ImVec2 vertices[8];
                    for (int i = 0; i < num_vertices; ++i)
                    {
                        b2Vec2 vertex = body->GetWorldPoint(polygon_shape->m_vertices[i]);
                        vertices[i] = ImVec2(vertex.x, vertex.y);
                    }
                    if (num_vertices == 3)
                    {
                        draw_list->AddTriangleFilled(vertices[0], vertices[1], vertices[2], IM_COL32(255, 0, 0, 255));
                    }
                    else if (num_vertices == 4)
                    {
                        draw_list->AddRectFilled(vertices[0], vertices[2], IM_COL32(255, 0, 0, 255));
                    }
                    else
                    {
                        draw_list->AddConvexPolyFilled(vertices, num_vertices, IM_COL32(255, 0, 0, 255));
                    }
                }
            }
        }
        // update the world
        world->Step(1.0f / 60.0f, 6, 2);



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

