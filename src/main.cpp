#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "texture.hpp"
#include "object.hpp"

#include <iostream>
#include <vector>
#include <regex>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    std::string vertexCode = shader::getShaderCode("shaders/vertex.vs");
    std::string fragmentCode = shader::getShaderCode("shaders/fragment.fs");

    unsigned int shaderProgram = shader::createShaderProgram(vertexCode.c_str(), fragmentCode.c_str());
    shader::VertexLayout vertexLayout = shader::parseVertexShaderCode(vertexCode.c_str());

    std::vector<float> vertices = {
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 1.0f,
    };

    std::vector<float> vertices2 = {
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    };

    std::vector<unsigned int> indices = {
        0, 1, 2
    };

    std::vector<unsigned int> indices2 = {
        0, 4, 1,
        1, 4, 2
    };

    Object::Object triangle(vertexLayout, vertices, indices);
    triangle.setShaderProgram(shaderProgram);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        triangle.draw();
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            triangle.bind(vertices2);
            triangle.bind(indices2);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}