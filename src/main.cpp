#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {

    // init GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Project3 Roller Coaster", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // init GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // init ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // ImGui GLFW and OpenGL3 initailization
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460"); // OpenGL version

    // Get OpenGL version information
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "OpenGL version: " << version << std::endl;

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // set background color
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // set black background

    // render loop
    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT);

        // codes
        // ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui drawing codes
        ImGui::Begin("Hello, ImGui!");
        ImGui::Text("This is a simple ImGui window.");
        ImGui::End();

        // render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // swap buffer and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
