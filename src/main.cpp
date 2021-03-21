#include "stdafx.h"
#include "FPSLimiter.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Main Window", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[] = {
        //position         //colors       //texcoords
       -0.5f, -0.5f, 0.0f, 1.f, 0.f, 0.f, 0.f, 0.f,
        0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f, 1.f, 0.f,
       -0.5f,  0.5f, 0.0f, 0.f, 0.f, 1.f, 0.f, 1.f,
        0.5f,  0.5f, 0.0f, 1.f, 1.f, 1.f, 1.f, 1.f
    };

    uint indices[] = {
        0, 1, 2,
        2, 1, 3
    };

    uint VAO;
    glGenVertexArrays(1, &VAO);

    uint texture = Texture::loadFromFile("resources/wall.jpg", false);

    uint VBO;
    glGenBuffers(1, &VBO);

    uint EBO;
    glGenBuffers(1, &EBO);

    ShaderProgram shaderProgram("shaders/default.vert", "shaders/default.frag");

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glUseProgram(shaderProgram.ID);
    glUniform1i(glGetUniformLocation(shaderProgram.ID, "tex"), 0);

    FPSLimiter limiter(60);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram.ID);
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, texture);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        limiter.update();
    }

    glfwTerminate();
    return 0;
}