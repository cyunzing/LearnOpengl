#define GLEW_STATIC
#include "gl/glew.h"
#include "GLFW/glfw3.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../../common/shader.h"
#include "../../common/texture.h"
#include "../../common/camera.h"

#define WINDOW_SIZE 500

Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));
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

    GLFWwindow *window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "Depth test", NULL, NULL);
    if (!window) {
        std::cout << "Error::GLFW could not create winddow!" << std::endl;
        glfwTerminate();
        std::system("pause");
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
        std::system("pause");
        return -1;
    }

    glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);

    // 指定顶点属性数据 顶点位置 纹理
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	// A
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,	// B
        0.5f, 0.5f, 0.5f,1.0f, 1.0f,	// C
        0.5f, 0.5f, 0.5f,1.0f, 1.0f,	// C
        -0.5f, 0.5f, 0.5f,0.0f, 1.0f,	// D
        -0.5f, -0.5f, 0.5f,0.0f, 0.0f,	// A


        -0.5f, -0.5f, -0.5f,0.0f, 0.0f,	// E
        -0.5f, 0.5f, -0.5f,0.0, 1.0f,   // H
        0.5f, 0.5f, -0.5f,1.0f, 1.0f,	// G
        0.5f, 0.5f, -0.5f,1.0f, 1.0f,	// G
        0.5f, -0.5f, -0.5f,1.0f, 0.0f,	// F
        -0.5f, -0.5f, -0.5f,0.0f, 0.0f,	// E

        -0.5f, 0.5f, 0.5f,0.0f, 1.0f,	// D
        -0.5f, 0.5f, -0.5f,1.0, 1.0f,   // H
        -0.5f, -0.5f, -0.5f,1.0f, 0.0f,	// E
        -0.5f, -0.5f, -0.5f,1.0f, 0.0f,	// E
        -0.5f, -0.5f, 0.5f,0.0f, 0.0f,	// A
        -0.5f, 0.5f, 0.5f,0.0f, 1.0f,	// D

        0.5f, -0.5f, -0.5f,1.0f, 0.0f,	// F
        0.5f, 0.5f, -0.5f,1.0f, 1.0f,	// G
        0.5f, 0.5f, 0.5f,0.0f, 1.0f,	// C
        0.5f, 0.5f, 0.5f,0.0f, 1.0f,	// C
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	// B
        0.5f, -0.5f, -0.5f,1.0f, 0.0f,	// F

        0.5f, 0.5f, -0.5f,1.0f, 1.0f,	// G
        -0.5f, 0.5f, -0.5f,0.0, 1.0f,   // H
        -0.5f, 0.5f, 0.5f,0.0f, 0.0f,	// D
        -0.5f, 0.5f, 0.5f,0.0f, 0.0f,	// D
        0.5f, 0.5f, 0.5f,1.0f, 0.0f,	// C
        0.5f, 0.5f, -0.5f,1.0f, 1.0f,	// G

        -0.5f, -0.5f, 0.5f,0.0f, 0.0f,	// A
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,// E
        0.5f, -0.5f, -0.5f,1.0f, 1.0f,	// F
        0.5f, -0.5f, -0.5f,1.0f, 1.0f,	// F
        0.5f, -0.5f, 0.5f,1.0f, 0.0f,	// B
        -0.5f, -0.5f, 0.5f,0.0f, 0.0f,	// A
    };

    // 地板顶点属性数据 顶点位置 纹理坐标(设置的值大于1.0用于重复)
    GLfloat planeVertices[] = {
        5.0f, -0.5f, 5.0f, 2.0f, 0.0f,   // A
        5.0f, -0.5f, -5.0f, 2.0f, 2.0f,  // D
        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f, // C

        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f, // C
        -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,  // B
        5.0f, -0.5f, 5.0f, 2.0f, 0.0f,   // A
    };

    // 透明物体的顶点属性  位置 纹理坐标
    GLfloat transparentVertices[] = {
        // 翻转了纹理y坐标 以便正立显示纹理
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
        1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
        1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.5f, 0.0f, 1.0f, 0.0f
    };

    // 透明物体位置
    std::vector<glm::vec3> vegetation;
    vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

    GLuint cubeVaoId, cubeVboId;
    glGenVertexArrays(1, &cubeVaoId);
    glBindVertexArray(cubeVaoId);
    glGenBuffers(1, &cubeVboId);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint planeVaoId, planeVboId;
    glGenVertexArrays(1, &planeVaoId);
    glBindVertexArray(planeVaoId);
    glGenBuffers(1, &planeVboId);
    glBindBuffer(GL_ARRAY_BUFFER, planeVboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint transparentVaoId, transparentVboId;
	glGenVertexArrays(1, &transparentVaoId);
	glBindVertexArray(transparentVaoId);
	glGenBuffers(1, &transparentVboId);
	glBindBuffer(GL_ARRAY_BUFFER, transparentVboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    GLuint cubeTexId = TextureHelper::load2DTexture("../../resources/textures/marble.jpg");
    GLuint planeTexId = TextureHelper::load2DTexture("../../resources/textures/metal.png");
	GLuint transparentTexId = TextureHelper::load2DTexture("../../resources/textures/grass.png", GL_RGBA, GL_RGBA, SOIL_LOAD_RGBA, true);

    Shader shader("cube.vert", "cube.frag");
	Shader transparentShader("transparent.vert", "transparent.frag");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    while (!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = (GLfloat)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();// 处理例如鼠标 键盘等事件
        movement();

        // 清除颜色缓冲区 深度缓冲区
        glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 绘制实际场景
        shader.use();
        glm::mat4 projection = glm::perspective(camera.mouseZoom, (GLfloat)(WINDOW_SIZE) / WINDOW_SIZE, 1.0f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.programId, "view"), 1, GL_FALSE, glm::value_ptr(view));

		transparentShader.use();
		glUniformMatrix4fv(glGetUniformLocation(transparentShader.programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(transparentShader.programId, "view"), 1, GL_FALSE, glm::value_ptr(view));

		shader.use();
        glBindVertexArray(cubeVaoId);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexId);
        glUniform1i(glGetUniformLocation(shader.programId, "tex"), 0);

        // 绘制第一个立方体
		glm::mat4 model;
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);// 这里的36是用于构成三角形的顶点个数 而不是三角形的个数12

        // 绘制第二个立方体
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 绘制平面
        glBindVertexArray(planeVaoId);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, planeTexId);
        glUniform1i(glGetUniformLocation(shader.programId, "tex"), 0);

        model = glm::mat4();
        glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 6);

		// 绘制透明物体
		transparentShader.use();
		glBindVertexArray(transparentVaoId);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, transparentTexId);
		for (std::vector<glm::vec3>::const_iterator it = vegetation.begin(); it != vegetation.end(); ++it) {
			model = glm::mat4();
			model = glm::translate(model, *it);
			glUniformMatrix4fv(glGetUniformLocation(transparentShader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}


        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &cubeVaoId);
    glDeleteVertexArrays(1, &planeVaoId);
	glDeleteVertexArrays(1, &transparentVaoId);
    glDeleteBuffers(1, &cubeVboId);
    glDeleteBuffers(1, &planeVboId);
	glDeleteBuffers(1, &transparentVaoId);

    glfwTerminate();

    return 0;
}
