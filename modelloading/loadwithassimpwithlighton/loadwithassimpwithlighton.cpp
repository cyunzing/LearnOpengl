#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/gtc/type_ptr.hpp"

#include "../../common/shader.h"
#include "../../common/texture.h"
#include "../../common/camera.h"
#include "../../common/model.h"

#define WINDOW_SIZE 500

Camera camera(glm::vec3(0.0f, 0.0f, 4.0f));
GLboolean keyStatus[1024] = { GL_FALSE };
GLfloat deltaTime = 0.0f, lastFrame = 0.0f;
GLboolean buttonPressed = GL_FALSE;
glm::vec3 lampPosition(0.8f, 0.8f, 0.5f);

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

    GLFWwindow *window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "Load model with assimp with light on", NULL, NULL);
    if (!window) {
        std::cout << "Error::GLFW could not create winddow!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyboard);
    glfwSetMouseButtonCallback(window, mousebutton);
    glfwSetCursorPosCallback(window, cursorpos);
    glfwSetScrollCallback(window, mousescoll);
    // 鼠标捕获 停留在程序内
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    GLenum status = glewInit();
    if (status != GLEW_OK) {
        std::cout << "Error::GLEW glew version:" << glewGetString(GLEW_VERSION)
            << " error string:" << glewGetErrorString(status) << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);

    //Section1 加载模型数据 为了方便更换模型 我们从文件读取模型文件路径
    Model obj;
#if 0
    std::ifstream modelPath("model-path.txt");
    if (!modelPath) {
        std::cerr << "Error::could not read model path file." << std::endl;
        glfwTerminate();
        std::system("pause");
        return -1;
    }

    std::string modelFilePath;
    std::getline(modelPath, modelFilePath);
    if (modelFilePath.empty()) {
        std::cerr << "Error::model path empty." << std::endl;
        glfwTerminate();
        std::system("pause");
        return -1;
    }
#else
    std::string modelFilePath = "../../resources/models/nanosuit/nanosuit.obj";
#endif
    if (!obj.loadModel(modelFilePath)) {
        std::cerr << "Could not load obj model." << std::endl;
        glfwTerminate();
        std::system("pause");
        return -1;
    }

    // Section2 准备着色器程序
    Shader shader("model.vert", "model.frag");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    while (!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = (GLfloat)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();// 处理例如鼠标 键盘等事件
        movement();

        glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

		// 设置光源属性 点光源
		GLfloat radius = 2.0f;
		lampPosition.x = radius * cos(glfwGetTime());
		lampPosition.z = radius * sin(glfwGetTime());
		glUniform3f(glGetUniformLocation(shader.programId, "light.position"), lampPosition.x, lampPosition.y, lampPosition.z);
		glUniform3f(glGetUniformLocation(shader.programId, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(shader.programId, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(shader.programId, "light.specular"), 1.0f, 1.0f, 1.0f);
		// 设置衰减系数
		glUniform1f(glGetUniformLocation(shader.programId, "light.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(shader.programId, "light.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(shader.programId, "light.quadratic"), 0.032f);
		// 设置观察者位置
		glUniform3f(glGetUniformLocation(shader.programId, "viewPosition"), camera.position.x, camera.position.y, camera.position.z);

        glm::mat4 projection = glm::perspective(camera.mouseZoom, (GLfloat)(WINDOW_SIZE) / WINDOW_SIZE, 1.0f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();

        glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.programId, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -1.55f, -1.0f)); // 适当调整位置
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f)); // 适当缩小模型
        glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));

        obj.draw(shader);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
