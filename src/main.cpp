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
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

static Object::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);

static unsigned int SCR_WIDTH = 800;
static unsigned int SCR_HEIGHT = 600;

static bool useMouse = true;

const float targetFPS = 60.0f;
const float targetTime = 1.0f / targetFPS;

static double deltaTime = 0.0f;

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    std::string vertexCode = shader::getShaderCode("shaders/vertex.vs");
    std::string fragmentCode = shader::getShaderCode("shaders/fragment.fs");

    std::string floorVertexCode = shader::getShaderCode("shaders/floor.vs");
    std::string floorFragmentCode = shader::getShaderCode("shaders/floor.fs");

    unsigned int shaderProgram = shader::createShaderProgram(vertexCode.c_str(), fragmentCode.c_str());
    unsigned int floorShaderProgram = shader::createShaderProgram(floorVertexCode.c_str(), floorFragmentCode.c_str());
    shader::VertexLayout vertexLayout = shader::parseVertexShaderCode(vertexCode.c_str());
    shader::VertexLayout floorVertexLayout = shader::parseVertexShaderCode(floorVertexCode.c_str());

    std::vector<float> vertices = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35
    };

    // Square vertices 4x4 grid mesh mini
    std::vector<float> floorVertices = {
    -1.0f, -1.0f, 0.0f,    1.0f, 0.0f, 0.0f,  // Bottom-left (Red)
    -0.5f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f,  // Bottom-center-left (Green)
    0.0f, -1.0f, 0.0f,     0.0f, 0.0f, 1.0f,  // Bottom-center-right (Blue)
    0.5f, -1.0f, 0.0f,     1.0f, 1.0f, 0.0f,  // Bottom-right (Yellow)
    1.0f, -1.0f, 0.0f,     1.0f, 0.0f, 1.0f,  // Far-right (Magenta)

    -1.0f, -0.5f, 0.0f,    0.0f, 1.0f, 1.0f,  // Mid-left (Cyan)
    -0.5f, -0.5f, 0.0f,    1.0f, 0.5f, 0.0f,  // Mid-center-left (Orange)
    0.0f, -0.5f, 0.0f,     0.5f, 0.0f, 0.5f,  // Mid-center-right (Purple)
    0.5f, -0.5f, 0.0f,     0.5f, 0.5f, 0.5f,  // Mid-right (Gray)
    1.0f, -0.5f, 0.0f,     0.0f, 0.0f, 0.0f,  // Far-right (Black)

    -1.0f, 0.0f, 0.0f,     1.0f, 0.0f, 0.0f,  // Center-left (Red)
    -0.5f, 0.0f, 0.0f,     0.0f, 1.0f, 0.0f,  // Center-center-left (Green)
    0.0f, 0.0f, 0.0f,      0.0f, 0.0f, 1.0f,  // Center-center-right (Blue)
    0.5f, 0.0f, 0.0f,      1.0f, 1.0f, 0.0f,  // Center-right (Yellow)
    1.0f, 0.0f, 0.0f,      1.0f, 0.0f, 1.0f,  // Far-right (Magenta)

    -1.0f, 0.5f, 0.0f,     0.0f, 1.0f, 1.0f,  // Top-left (Cyan)
    -0.5f, 0.5f, 0.0f,     1.0f, 0.5f, 0.0f,  // Top-center-left (Orange)
    0.0f, 0.5f, 0.0f,      0.5f, 0.0f, 0.5f,  // Top-center-right (Purple)
    0.5f, 0.5f, 0.0f,      0.5f, 0.5f, 0.5f,  // Top-right (Gray)
    1.0f, 0.5f, 0.0f,      0.0f, 0.0f, 0.0f,  // Far-right (Black)

    -1.0f, 1.0f, 0.0f,     1.0f, 0.0f, 0.0f,  // Far-top-left (Red)
    -0.5f, 1.0f, 0.0f,     0.0f, 1.0f, 0.0f,  // Far-top-center-left (Green)
    0.0f, 1.0f, 0.0f,      0.0f, 0.0f, 1.0f,  // Far-top-center-right (Blue)
    0.5f, 1.0f, 0.0f,      1.0f, 1.0f, 0.0f,  // Far-top-right (Yellow)
    1.0f, 1.0f, 0.0f,      1.0f, 0.0f, 1.0f
    };

    std::vector<unsigned int> floorIndices = {
    0, 1, 5, 1, 6, 5,  // First quad
    1, 2, 6, 2, 7, 6,  // Second quad
    2, 3, 7, 3, 8, 7,  // Third quad
    3, 4, 8, 4, 9, 8,  // Fourth quad

    5, 6, 10, 6, 11, 10,  // Fifth quad
    6, 7, 11, 7, 12, 11,  // Sixth quad
    7, 8, 12, 8, 13, 12,  // Seventh quad
    8, 9, 13, 9, 14, 13,  // Eighth quad

    10, 11, 15, 11, 16, 15,  // Ninth quad
    11, 12, 16, 12, 17, 16,  // Tenth quad
    12, 13, 17, 13, 18, 17,  // Eleventh quad
    13, 14, 18, 14, 19, 18,  // Twelfth quad

    15, 16, 20, 16, 21, 20,  // Thirteenth quad
    16, 17, 21, 17, 22, 21,  // Fourteenth quad
    17, 18, 22, 18, 23, 22,  // Fifteenth quad
    18, 19, 23, 19, 24, 23   // Sixteenth quad
    };

    Object::Mesh sphereMesh = Object::Shape::createSphere(0.5f, 35);

    std::vector<unsigned int> sphereIndices = sphereMesh.indexData();
    std::vector<float> sphereVertices = sphereMesh.vertexData();


    Object::DataObject floor(floorVertexLayout, floorShaderProgram, floorVertices, floorIndices);
    Object::DataObject sphere(floorVertexLayout, floorShaderProgram, sphereVertices, sphereIndices);
    Object::Texture texture(vertexLayout, shaderProgram, vertices, indices, "src/linus2.jpeg");

    glUseProgram(texture.getShaderProgram());
    unsigned int ourTexture = glGetUniformLocation(texture.getShaderProgram(), "ourTexture");
    glUniform1i(ourTexture, 0);

    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = camera.getProjectionMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    unsigned int projectionLoc = glGetUniformLocation(texture.getShaderProgram(), "projection");
    unsigned int viewLoc = glGetUniformLocation(texture.getShaderProgram(), "view");
    unsigned int modelLoc = glGetUniformLocation(texture.getShaderProgram(), "model");

    glm::mat4 floorProjection = glm::mat4(1.0f);
    glm::mat4 floorModel = glm::mat4(1.0f);

    floorModel = glm::rotate(floorModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    floorModel = glm::translate(floorModel, glm::vec3(0.0f, 0.0f, -1.0f));
    floorModel = glm::scale(floorModel, glm::vec3(5.0f, 5.0f, 5.0f));
    floorProjection = camera.getProjectionMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    unsigned int floorProjectionLoc = glGetUniformLocation(floor.getShaderProgram(), "projection");
    unsigned int floorViewLoc = glGetUniformLocation(floor.getShaderProgram(), "view");
    unsigned int floorModelLoc = glGetUniformLocation(floor.getShaderProgram(), "model");

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        static double lastFrameTime = 0.0;
        double currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;

        if (deltaTime < targetTime) {
            glfwWaitEventsTimeout(targetTime - deltaTime);
            currentFrameTime = glfwGetTime();
            deltaTime = currentFrameTime - lastFrameTime;
        }
        
        lastFrameTime = currentFrameTime;   
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model = glm::rotate(model, (float)deltaTime * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        view = camera.getViewMatrix();
        projection = camera.getProjectionMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        texture.draw();

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        floor.draw();

        sphere.draw();

        glUniformMatrix4fv(floorProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(floorViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(floorModelLoc, 1, GL_FALSE, glm::value_ptr(floorModel));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    static bool wasEscDown = false;
    static bool wasTabDown = false;

    static bool isWireframe = false;

    bool isMouseDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (isMouseDown) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        useMouse = true;
    }

    bool isEscDown = glfwGetKey(window, GLFW_KEY_ESCAPE);
    if (isEscDown && !wasEscDown) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        useMouse = false;
    }
    wasEscDown = isEscDown;
    
    bool isTabDown = glfwGetKey(window, GLFW_KEY_TAB);
    if (isTabDown && !wasTabDown) {
        isWireframe = !isWireframe;
        if (isWireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    }
    wasTabDown = isTabDown;

    float cameraSpeed = 2.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.translate(cameraSpeed * camera.getFront());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.translate(-cameraSpeed * camera.getFront());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.translate(-glm::normalize(glm::cross(camera.getFront(), camera.getUp())) * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.translate(glm::normalize(glm::cross(camera.getFront(), camera.getUp())) * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.translate(-cameraSpeed * camera.getUp());
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.translate(cameraSpeed * camera.getUp());
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!useMouse) { return; }
    static bool firstMouse = true;
    static float lastX = SCR_WIDTH / 2.0f;
    static float lastY = SCR_HEIGHT / 2.0f;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    static float yaw   = -90.0f; // yaw is initialized to -90.0 degrees
    static float pitch = 0.0f;

    yaw   += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera.setFront(glm::normalize(front));
}