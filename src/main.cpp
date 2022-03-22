#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include <iostream>
#include <string>

int main() {
    //Initializing GLFW:
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Creating a window:
    constexpr int width = 800, height = 600;
    const std::string title = "LennardJones";
    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if (window == nullptr) {
        glfwTerminate();
        std::cout << "Failed to initialize GLFW" << '\n';
        return -1;
    }

    //Initializing GLAD:
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << '\n';
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}