//
// Created by Karol Hetman on 27/02/2023.
//

#include "GUI.h"



int GUI::Init() {

}

void GUI::Exit() {
    ImGui::DestroyContext();
}

GUI::~GUI() {
    Exit();
}


        void GUI::simpleWindow() {

            ImGui::Begin( "HI?");                          // Create a window called "Hello, world!" and append into it.
            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

void GUI::simpleWindow2() {

    ImGui::Begin( "HI?2");                          // Create a window called "Hello, world!" and append into it.
    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();


}