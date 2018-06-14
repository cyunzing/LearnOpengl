#define GLEW_STATIC
#include "gl/glew.h"
#include "GLFW/glfw3.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <sstream>

#include "../../common/shader.h"
#include "../../common/texture.h"
#include "../../common/camera.h"

#define WINDOW_SIZE 500

Camera camera(glm::vec3(0.0f, 0.0f, 4.0f));
GLboolean keyStatus[1024] = { GL_FALSE };
GLfloat deltaTime = 0.0f, lastFrame = 0.0f;
GLboolean buttonPressed = GL_FALSE;

void keyboard(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            keyStatus[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            keyStatus[key] = GL_FALSE;
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void mousebutton(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS)
            buttonPressed = GL_TRUE;
        else if (action == GLFW_RELEASE)
            buttonPressed = GL_FALSE;
    }
}

void cursorpos(GLFWwindow *window, double xpos, double ypos)
{
    static GLboolean firstMove = GL_TRUE;
    static double lastX = 0.0, lastY = 0.0;

    if (buttonPressed == GL_FALSE) {
        firstMove = GL_TRUE;
        return;
    }

    if (firstMove) {// 首次鼠标移动
        lastX = xpos;
        lastY = ypos;
        firstMove = GL_FALSE;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = ypos - lastY;

    lastX = xpos;
    lastY = ypos;

    camera.handleMouseMove(xoffset, yoffset);
}

// 由相机辅助类处理鼠标滚轮控制
void mousescoll(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.handleMouseScroll(yoffset);
}

void movement()
{
    if (keyStatus[GLFW_KEY_W])
        camera.handleKeyPress(Forward, deltaTime);
    if (keyStatus[GLFW_KEY_S])
        camera.handleKeyPress(Backward, deltaTime);
    if (keyStatus[GLFW_KEY_A])
        camera.handleKeyPress(Left, deltaTime);
    if (keyStatus[GLFW_KEY_D])
        camera.handleKeyPress(Right, deltaTime);
}

int main(int argc, char *argv[])
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "Spot light soft edge", NULL, NULL);
    if (!window)
        return -1;

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyboard);
    glfwSetMouseButtonCallback(window, mousebutton);
    glfwSetCursorPosCallback(window, cursorpos);
    glfwSetScrollCallback(window, mousescoll);
    // 鼠标捕获 停留在程序内
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        return -1;

    glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);

    // 指定顶点属性数据 顶点位置 纹理 法向量
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,1.0f,	// A
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,	// B
        0.5f, 0.5f, 0.5f,  1.0f, 1.0f,   0.0f, 0.0f, 1.0f,	// C
        0.5f, 0.5f, 0.5f,  1.0f, 1.0f,   0.0f, 0.0f, 1.0f,	// C
        -0.5f, 0.5f, 0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,	// D
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// A


        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,	// E
        -0.5f, 0.5f, -0.5f,  0.0, 1.0f,  0.0f, 0.0f, -1.0f, // H
        0.5f, 0.5f, -0.5f,   1.0f, 1.0f, 0.0f, 0.0f, -1.0f,	// G
        0.5f, 0.5f, -0.5f,   1.0f, 1.0f, 0.0f, 0.0f, -1.0f,	// G
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,	// F
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,	// E

        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,	// D
        -0.5f, 0.5f, -0.5f, 1.0, 1.0f,   -1.0f, 0.0f, 0.0f, // H
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,	// E
        -0.5f, -0.5f, -0.5f,1.0f, 0.0f, -1.0f, 0.0f, 0.0f,	// E
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,	// A
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,	// D

        0.5f, -0.5f, -0.5f,1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // F
        0.5f, 0.5f, -0.5f,1.0f, 1.0f,  1.0f, 0.0f, 0.0f, // G
        0.5f, 0.5f, 0.5f,0.0f, 1.0f,   1.0f, 0.0f, 0.0f, // C
        0.5f, 0.5f, 0.5f,0.0f, 1.0f,   1.0f, 0.0f, 0.0f, // C
        0.5f, -0.5f, 0.5f,0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // B
        0.5f, -0.5f, -0.5f,1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // F

        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// G
        -0.5f, 0.5f, -0.5f, 0.0, 1.0f, 0.0f, 1.0f, 0.0f,    // H
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,	// D
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,	// D
        0.5f, 0.5f, 0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,	// C
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// G

        -0.5f, -0.5f, 0.5f,0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  // A
        -0.5f, -0.5f, -0.5f,0.0f, 1.0f, 0.0f, -1.0f, 0.0f,  // E
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  0.0f, -1.0f, 0.0f, // F
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  0.0f, -1.0f, 0.0f, // F
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   0.0f, -1.0f, 0.0f, // B
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f, // A
    };

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)
    };

    GLuint vaoId, vboId;
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid *)(5 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader shader("cube.vert", "cube.frag");
    Shader lampShader("lamp.vert", "lamp.frag");

    GLuint diffuseMap = TextureHelper::load2DTexture("../../resources/textures/container_diffuse.png");
    GLuint specularMap = TextureHelper::load2DTexture("../../resources/textures/container_diffuse.png");
    shader.use();

    glUniform1i(glGetUniformLocation(shader.programId, "material.diffuseMap"), 0);
    glUniform1i(glGetUniformLocation(shader.programId, "material.specularMap"), 1);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = (GLfloat)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();// 处理例如鼠标 键盘等事件
        movement();

        glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(camera.mouseZoom, (GLfloat)(WINDOW_SIZE) / WINDOW_SIZE, 1.0f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();

        glBindVertexArray(vaoId);
        shader.use();

        // 设置方向光源
        glm::vec3 lampDirection(0.5f, 0.8f, 0.0f);
        glUniform3f(glGetUniformLocation(shader.programId, "dlight.ambient"), 0.0f, 0.1f, 0.4f);
        glUniform3f(glGetUniformLocation(shader.programId, "dlight.diffuse"), 0.0f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(shader.programId, "dlight.specular"), 0.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(shader.programId, "dlight.direction"), lampDirection.x, lampDirection.y, lampDirection.z); // 方向光源

        // 设置点光源
        for (int i = 0; i < sizeof(pointLightPositions) / sizeof(pointLightPositions[0]); ++i) {
            // 设置光源属性 点光源
            std::stringstream idx;
            idx << i;
            std::string lightName = "plights[" + idx.str() + "]";  // 使用数组索引plights[i]
            glUniform3f(glGetUniformLocation(shader.programId, (lightName + ".ambient").c_str()), 0.0f, 0.1f, 0.4f);
            glUniform3f(glGetUniformLocation(shader.programId, (lightName + ".diffuse").c_str()), 0.0f, 0.1f, 0.6f);
            glUniform3f(glGetUniformLocation(shader.programId, (lightName + ".specular").c_str()), 0.0f, 1.0f, 1.0f);
            glm::vec3 lampPos = pointLightPositions[i];
            glUniform3f(glGetUniformLocation(shader.programId, (lightName + ".position").c_str()), lampPos.x, lampPos.y, lampPos.z);
            // 设置衰减系数
            glUniform1f(glGetUniformLocation(shader.programId, (lightName + ".constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(shader.programId, (lightName + ".linear").c_str()), 0.09f);
            glUniform1f(glGetUniformLocation(shader.programId, (lightName + ".quadratic").c_str()), 0.032f);
        }

        // 设置光源属性 FlashLight光源
        glUniform3f(glGetUniformLocation(shader.programId, "slight.ambient"), 0.0f, 0.1f, 0.4f);
        glUniform3f(glGetUniformLocation(shader.programId, "slight.diffuse"), 0.0f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(shader.programId, "slight.specular"), 0.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(shader.programId, "slight.position"), camera.position.x, camera.position.y, camera.position.z);
        glUniform3f(glGetUniformLocation(shader.programId, "slight.direction"), camera.forward.x, camera.forward.y, camera.forward.z);
        glUniform1f(glGetUniformLocation(shader.programId, "slight.cutoff"), cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(shader.programId, "slight.outerCutoff"), cos(glm::radians(17.5f)));
        // 设置衰减系数
        glUniform1f(glGetUniformLocation(shader.programId, "slight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shader.programId, "slight.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(shader.programId, "slight.quadratic"), 0.032f);

        // 设置材料光照属性
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glUniform1f(glGetUniformLocation(shader.programId, "material.shininess"), 32.0f);

        // 设置观察者位置
        glUniform3f(glGetUniformLocation(shader.programId, "viewPosition"), camera.position.x, camera.position.y, camera.position.z);

        // 设置变换矩阵
        glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.programId, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 model;
        for (int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); ++i) {
            model = glm::mat4();
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = 20.0f * i;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vaoId);
    glDeleteBuffers(1, &vboId);

    glfwTerminate();

    return 0;
}
