#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#define WINDOW_SIZE 500

void keyboard(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(int argc, char *argv[])
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "glfw context", NULL, NULL);
    if (!window)
        return -1;

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, keyboard);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
        return -1;

    glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
