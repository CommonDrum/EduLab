//
// Created by Karol Hetman on 27/02/2023.
//

#include "GUI.h"




int GUI::Init() {


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = ImGui::GetIO(); (void)m_io;

    // Setup Dear ImGui context

    m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    m_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    m_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows


    // Setup Dear ImGui m_style
    ImGui::StyleColorsDark();


    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    m_style = ImGui::GetStyle();
    if (m_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        m_style.WindowRounding = 0.0f;
        m_style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(m_window, m_glContext);
    ImGui_ImplOpenGL3_Init(glsl_version);




    return 0;
}

void GUI::Exit() {
    ImGui::DestroyContext();
}

GUI::~GUI() {
    Exit();
}

void GUI::Render() {
    {




            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            if (m_show_demo_window)
                ImGui::ShowDemoWindow(&m_show_demo_window);

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin(
                        "Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text(
                        "This is some useful text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window",
                                &m_show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &m_show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float *) &m_clear_color); // Edit 3 floats representing a color

                if (ImGui::Button(
                        "Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                            ImGui::GetIO().Framerate);
                ImGui::End();
            }

            // 3. Show another simple window.
            if (m_show_another_window) {
                ImGui::Begin("Another Window",
                             &m_show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImGui::Text("Hello from another window!");
                if (ImGui::Button("Close Me"))
                    m_show_another_window = false;
                ImGui::End();
            }

            // Rendering
            ImGui::Render();
            glViewport(0, 0, (int) m_io.DisplaySize.x, (int) m_io.DisplaySize.y);
            glClearColor(m_clear_color.x * m_clear_color.w, m_clear_color.y * m_clear_color.w,
                         m_clear_color.z * m_clear_color.w, m_clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Update and Render additional Platform Windows
            // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
            //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
            if (m_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
                SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
            }

            SDL_GL_SwapWindow(m_window);

    }
}

GUI::GUI(SDL_Window* window, SDL_GLContext glContext,const char* glslVersion,SDL_Event* event) {
    m_window = window;
    m_glContext = glContext;
    glsl_version = glslVersion;
    m_event = event;


}

void GUI::PollEvents() {
    ImGui_ImplSDL2_ProcessEvent(m_event);
    if (m_event->type == SDL_QUIT)
        done = true;
    if (m_event->type == SDL_WINDOWEVENT && m_event->window.event == SDL_WINDOWEVENT_CLOSE &&
        m_event->window.windowID == SDL_GetWindowID(m_window))
        done = true;

}
