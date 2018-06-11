#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "../common/shader.h"

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

    GLFWwindow *window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "Triangle Colored", NULL, NULL);
    if (!window)
        return -1;

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, keyboard);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
        return -1;

    glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);

    GLfloat vertices[] = {
        -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    GLuint vaoId, vboId;
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader shader("triangle_colored.vert", "triangle_colored.frag");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vaoId);
        shader.use();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vaoId);
    glDeleteBuffers(1, &vboId);

    glfwTerminate();

    return 0;
}
