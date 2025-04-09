#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
        // positions       // color           // texture coords
        0.5f, 0.5f,        1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
        0.5f, -0.5f,       1.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom right
        -0.5f, -0.5f,      0.0f, 1.0f, 0.0f,  0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,       0.0f, 0.0f, 1.0f,  0.0f, 1.0f  // top left
    };

    std::vector<unsigned int> indices = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    Object::Texture texture(vertexLayout, shaderProgram, vertices, indices, "src/tomato.png");
    unsigned int ourTexture = glGetUniformLocation(texture.getShaderProgram(), "ourTexture");
    std::cout << "ourTexture: " << ourTexture << std::endl;
    glUniform1i(ourTexture, 0);

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    unsigned int transformLoc = glGetUniformLocation(texture.getShaderProgram(), "transform");
    std::cout << "transformLoc: " << transformLoc << std::endl;
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        texture.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    static bool wasWDown = false;
    static bool isWireframe = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    bool isWDown = glfwGetKey(window, GLFW_KEY_W);
    if (isWDown && !wasWDown) {
        isWireframe = !isWireframe;
        if (isWireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    }
    wasWDown = isWDown;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}