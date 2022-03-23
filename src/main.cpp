#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "Shader.h"

int main() {
    //Initializing GLFW:
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Creating a window:
    constexpr int width = 800, height = 600;
    constexpr char* title = "LennardJones";
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);

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

    unsigned int vao, vbo;
    float vertex_data[24] = {-0.5f, -0.5f, -1.0f, -1.0f,
                              0.5f, -0.5f,  1.0f, -1.0f,
                              0.5f,  0.5f,  1.0f,  1.0f,
                             -0.5f, -0.5f, -1.0f, -1.0f,
                              0.5f,  0.5f,  1.0f,  1.0f,
                             -0.5f,  0.5f, -1.0f,  1.0f};

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader shader("shaders/QuadPoints.vs", "shaders/QuadPoints.fs");


    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Bind();

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    return 0;
}