#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"

#include "wave.h"
#include "skybox.h"
#include "environment.h"
#include "pixelization.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool mouseControlEnabled = true;
bool mouseControlButtonPressed = false;

int main() {

    //================================================================================================
    // init codes
    //================================================================================================
    // init GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Introduction to Computer Graphics", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // init GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
        std::cerr << "Failed to initialize ImGui GLFW" << std::endl;
        return -1;
    }
    if (!ImGui_ImplOpenGL3_Init("#version 460")) {
        std::cerr << "Failed to initialize ImGui OpenGL3" << std::endl;
        return -1;
    }
    std::cout << "ImGui initialized successfully" << std::endl;

    // Get OpenGL version information
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "OpenGL version: " << version << std::endl;

    //================================================================================================
    // OpenGL codes
    //================================================================================================

    glEnable(GL_DEPTH_TEST);

    // HW10: Waves
    Wave wave(100, 100);
    Shader waterShader("shaders/wave.vs", "shaders/wave.fs");

    // HW11: Skybox
    std::vector<std::string>faces {
        "resources/skybox/right.jpg",
        "resources/skybox/left.jpg",
        "resources/skybox/top.jpg",
        "resources/skybox/bottom.jpg",
        "resources/skybox/front.jpg",
        "resources/skybox/back.jpg"
    };
    Skybox skybox(faces);
    Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs");

    Environment environment;

    // Post-processing
    Pixelization pixelization(SCR_WIDTH, SCR_HEIGHT);

    // Light
    Light directionalLight(DIRECTIONAL, glm::vec3(0.0f), glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(1.0f, 1.0f, 1.0f));
    Light pointLight(POINT, glm::vec3(30.0f, 1.0f, 30.0f), glm::vec3(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    Light spotLight(SPOT, camera.Position, camera.Front, glm::vec3(1.0f, 1.0f, 1.0f));

    //================================================================================================
    // render loop
    //================================================================================================
    while (!glfwWindowShouldClose(window)) {

        //================================================================================================
        // OpenGL codes
        //================================================================================================
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
    
        // set background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Begin post-processing render
        pixelization.beginRender();

        // projection
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // cam view
        glm::mat4 view = camera.GetViewMatrix();

        // HW11: Skybox
        skyboxShader.use();
        glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix())); // 移除平移部分
        skyboxShader.setMat4("view", skyboxView);
        skyboxShader.setMat4("projection", projection);
        skybox.render(skyboxView, projection);

        // HW10: Waves
        wave.update(deltaTime);
        waterShader.use();
        // Set light and view pos
        directionalLight.apply(waterShader);
        pointLight.apply(waterShader);
        spotLight.apply(waterShader);
        waterShader.setVec3("viewPos", camera.Position);
        waterShader.setVec3("objectColor", wave.color);
        waterShader.setInt("skybox", 0);
        //set model and projection matrix
        glm::mat4 waveModel = glm::mat4(1.0f);
        waveModel = glm::translate(waveModel, glm::vec3(-(wave.width / 2.0f), -20.0f, -(wave.height / 2.0f)));
        waterShader.setMat4("model", waveModel);
        waterShader.setMat4("view", view);
        waterShader.setMat4("projection", projection);
        // render wave
        wave.render();

        environment.render(view, projection, camera.Position, skybox.getCubemapTexture(), currentFrame);

        // End post-processing render
        pixelization.endRender();

        // Render post-processing effect
        pixelization.render();

        //================================================================================================
        // ImGUI
        //================================================================================================
        // ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui drawing codes
        ImGui::Begin("Control Panel");
        ImGui::Text("Introduction to Computer Graphics");

        // User Guide
        ImGui::BeginChild("User Guide", ImVec2(0, 100), true);
        ImGui::Text("User Guide");
        ImGui::Text("M: Enable/Disable Mouse Camera Control");
        ImGui::Text("W: Move Forward");
        ImGui::Text("S: Move Backward");
        ImGui::Text("A: Move Left");
        ImGui::Text("D: Move Right");
        ImGui::Text("Space: Move Up");
        ImGui::Text("Left Control: Move Down");
        ImGui::EndChild();

        // Pixelization Control
        ImGui::BeginChild("Pixelization Control", ImVec2(0, 100), true);
        ImGui::Text("Pixelization Control");
        ImGui::SliderFloat("Pixel Size", &pixelization.pixelSize, 1.0f, 20.0f);
        ImGui::EndChild();

        // Create a child window for controlling wave properties
        ImGui::BeginChild("Wave Control", ImVec2(0, 100), true);
        ImGui::Text("Wave Control");
        ImGui::SliderFloat("Wave Speed", &wave.speed, 0.1f, 10.0f);
        ImGui::SliderFloat("Wave Amplitude", &wave.amplitude, 0.0f, 10.0f);
        ImGui::ColorEdit3("Wave Color", glm::value_ptr(wave.color));
        ImGui::SliderFloat2("Wave Direction", glm::value_ptr(wave.direction), -1.0f, 1.0f);
        ImGui::EndChild();

        // Create a child window for controlling light properties
        ImGui::BeginChild("Light Control", ImVec2(0, 200), true);
        ImGui::Text("Directional Light");
        ImGui::ColorEdit3("Dir Light Color", glm::value_ptr(directionalLight.color));
        ImGui::SliderFloat3("Dir Light Direction", glm::value_ptr(directionalLight.direction), -1.0f, 1.0f);

        ImGui::Text("Point Light");
        ImGui::ColorEdit3("Point Light Color", glm::value_ptr(pointLight.color));
        ImGui::SliderFloat3("Point Light Position", glm::value_ptr(pointLight.position), -50.0f, 50.0f);

        ImGui::Text("Spot Light");
        ImGui::ColorEdit3("Spot Light Color", glm::value_ptr(spotLight.color));
        ImGui::SliderFloat3("Spot Light Position", glm::value_ptr(spotLight.position), -50.0f, 50.0f);
        ImGui::SliderFloat3("Spot Light Direction", glm::value_ptr(spotLight.direction), -1.0f, 1.0f);
        ImGui::EndChild();

        ImGui::End();

        // render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // swap buffer and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //================================================================================================
    // clean up codes
    //================================================================================================
    // clean up ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // clean up GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    // Switch mouse control
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        if (!mouseControlButtonPressed) {
            mouseControlEnabled = !mouseControlEnabled;
            if (mouseControlEnabled) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            mouseControlButtonPressed = true;
        }
        
    } else {
        mouseControlButtonPressed = false;
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (!mouseControlEnabled) return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (!mouseControlEnabled) return;
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
