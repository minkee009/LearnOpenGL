#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
const char WINDOW_NAME[] = "Learn OpenGL - 01 : Create Window";

int main()
{
    SPDLOG_INFO("Start program");

    SPDLOG_INFO("Initialize glfw");
    if (!glfwInit())
    {
        const char* description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("Failed to initialize glfw: {}", description);
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    SPDLOG_INFO("Create glfw window");
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    SPDLOG_INFO("Start Main Loop");
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}