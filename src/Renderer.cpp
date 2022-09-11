#include "Renderer.h"

#include "glad/glad.h"

Renderer::Renderer(std::vector<Particle>* data) : m_InstanceData(data)
{
    Init();
}

Renderer::Renderer(std::vector<Particle>* data, float l) 
    : m_InstanceData(data), m_L(l)
{
    Init();
}

bool Renderer::ShouldClose() {
    return glfwWindowShouldClose(m_Window);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_InstanceVBO);

    delete m_Shader;
}

void Renderer::OnRender() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float aspect = float(m_WindowData.Width) / float(m_WindowData.Height);
    float min_res = static_cast<float>(std::min(m_WindowData.Width, m_WindowData.Height));

    float scale_x = (aspect < 1.0f) ? 1.0f : 1.0f / aspect;
    float scale_y = (aspect < 1.0f) ? aspect : 1.0f;

    float quad_size_mult = 2.0f * m_quadSize / min_res;
    float uv_smoothness = 2.0f * m_pxSmoothness / m_quadSize;

    m_Shader->Bind();
    m_Shader->setUniform1f("scale_x", scale_x);
    m_Shader->setUniform1f("scale_y", scale_y);
    m_Shader->setUniform1f("scale_mult", quad_size_mult);
    m_Shader->setUniform1f("L", m_L);
    m_Shader->setUniform1f("smoothness", uv_smoothness);

    glBindVertexArray(m_VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_InstanceCount);

    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

void Renderer::UpdateFromCPU(std::vector<Particle>* user_data) {
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * user_data->size(), &user_data->at(0), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::Init() {
    //Initializing GLFW:
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height,
        m_WindowData.Title.c_str(), NULL, NULL);

    glfwSetWindowUserPointer(m_Window, &m_WindowData);

    if (m_Window == nullptr) {
        glfwTerminate();
        throw "Failed to initialize GLFW";
    }

    //Initializing GLAD:
    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw "Failed to initialize GLAD";
    }

    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* win, int wdth, int hgth) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);
        data.Width = wdth;
        data.Height = hgth;
        glViewport(0, 0, wdth, hgth);
        });

    //Enable Vsync
    glfwSwapInterval(1);

    //Compile shaders after API initialization:
    m_Shader = new Shader("shaders/QuadPoints.vs", "shaders/QuadPoints.fs");

    //GL vertex data:
    m_InstanceCount = static_cast<unsigned int>(m_InstanceData->size());

    glGenBuffers(1, &m_InstanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * m_InstanceCount, &m_InstanceData->at(0), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    float quad_data[24] = { -0.5f, -0.5f, -1.0f, -1.0f,
                             0.5f, -0.5f,  1.0f, -1.0f,
                             0.5f,  0.5f,  1.0f,  1.0f,
                            -0.5f, -0.5f, -1.0f, -1.0f,
                             0.5f,  0.5f,  1.0f,  1.0f,
                            -0.5f,  0.5f, -1.0f,  1.0f };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribDivisor(2, 1);

    //GL settings:
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
