#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "Shader.h"

#include <random>

struct WindowData {
    std::string Title;
    unsigned int Width, Height;
};

int main() {
    //Initializing GLFW:
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Creating a window:
    WindowData window_data;
    window_data.Width = 800;
    window_data.Height = 600;
    window_data.Title = "LennardJones";

    GLFWwindow* window = glfwCreateWindow(window_data.Width, window_data.Height, 
                                          window_data.Title.c_str(), NULL, NULL);

    glfwSetWindowUserPointer(window, &window_data);

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

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int wdth, int hgth) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);
        data.Width = wdth;
        data.Height = hgth;
        glViewport(0, 0, wdth, hgth);
    });

    constexpr int num_of_points = 3000;
    float instance_pos[4 * num_of_points];

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(-1.0, 1.0);

    for (int i = 0; i < num_of_points; i++) {
        instance_pos[4 * i + 0] = dist(e2);
        instance_pos[4 * i + 1] = dist(e2);
        instance_pos[4 * i + 2] = 0.0f;
        instance_pos[4 * i + 3] = 0.0f;
    }

    unsigned int instance_vbo;

    glGenBuffers(1, &instance_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(instance_pos), instance_pos, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    float quad_data[24] = {-0.5f, -0.5f, -1.0f, -1.0f,
                            0.5f, -0.5f,  1.0f, -1.0f,
                            0.5f,  0.5f,  1.0f,  1.0f,
                           -0.5f, -0.5f, -1.0f, -1.0f,
                            0.5f,  0.5f,  1.0f,  1.0f,
                           -0.5f,  0.5f, -1.0f,  1.0f};

    unsigned int vao, vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glVertexAttribDivisor(2, 1);

    Shader shader("shaders/QuadPoints.vs", "shaders/QuadPoints.fs");
    constexpr float pixel_smoothness = 2.0f, quad_size_px = 10.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float aspect = float(window_data.Width) / float(window_data.Height);
        float min_res = static_cast<float>(std::min(window_data.Width, window_data.Height));

        float scale_x = (aspect < 1.0f) ? 1.0f : 1.0f / aspect;
        float scale_y = (aspect < 1.0f) ? aspect : 1.0f;

        float quad_size_mult = 2.0f * quad_size_px / min_res;
        float uv_smoothness = 2.0f * pixel_smoothness  / quad_size_px;

        shader.Bind();
        shader.setUniform1f("scale_x", scale_x);
        shader.setUniform1f("scale_y", scale_y);
        shader.setUniform1f("scale_mult", quad_size_mult);
        shader.setUniform1f("smoothness", uv_smoothness);

        glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, num_of_points);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    return 0;
}