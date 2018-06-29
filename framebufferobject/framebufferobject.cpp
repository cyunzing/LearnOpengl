#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../common/shader.h"
#include "../common/texture.h"
#include "../common/camera.h"

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

/*
* 附加纹理到Color, depth ,stencil Attachment
*/
bool prepareFBO1(GLuint &colorTexId, GLuint &depthStencilTexId, GLuint &fboId)
{
	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	// 附加 color attachment
	colorTexId = TextureHelper::makeAttachmentTexture(0, GL_RGB, WINDOW_SIZE, WINDOW_SIZE, GL_RGB, GL_UNSIGNED_BYTE);// 创建FBO中的纹理
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexId, 0);
	// 附加 depth stencil texture attachment
	depthStencilTexId = TextureHelper::makeAttachmentTexture(0, GL_DEPTH24_STENCIL8, WINDOW_SIZE, WINDOW_SIZE, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilTexId, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

/*
* 附加纹理到Color Attachment 同时附加RBO到depth stencil Attachment
*/
bool prepareFBO1(GLuint &colorTexId, GLuint &fboId)
{
	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	// 附加 color attachment
	colorTexId = TextureHelper::makeAttachmentTexture(0, GL_RGB, WINDOW_SIZE, WINDOW_SIZE, GL_RGB, GL_UNSIGNED_BYTE);// 创建FBO中的纹理
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexId, 0);
	// 附加 depth stencil RBO attachment
	GLuint rboId;
	glGenRenderbuffers(1, &rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_SIZE, WINDOW_SIZE);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboId);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "Frame buffer object", NULL, NULL);
    if (!window)
        return -1;

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyboard);
    glfwSetMouseButtonCallback(window, mousebutton);
    glfwSetCursorPosCallback(window, cursorpos);
    glfwSetScrollCallback(window, mousescoll);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return -1;
    }

    glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);

    // 指定立方体顶点属性数据 顶点位置 纹理
    GLfloat cubeVertices[] = {
        // 正面
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	// A
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,	// B
        0.5f, 0.5f, 0.5f,	1.0f, 1.0f,	// C
        0.5f, 0.5f, 0.5f,	1.0f, 1.0f,	// C
        -0.5f, 0.5f, 0.5f,	0.0f, 1.0f,	// D
        -0.5f, -0.5f, 0.5f,	0.0f, 0.0f,	// A

        // 背面
        -0.5f, -0.5f, -0.5f,0.0f, 0.0f,	// E
        -0.5f, 0.5f, -0.5f,0.0, 1.0f,   // H
        0.5f, 0.5f, -0.5f,1.0f, 1.0f,	// G
        0.5f, 0.5f, -0.5f,1.0f, 1.0f,	// G
        0.5f, -0.5f, -0.5f,1.0f, 0.0f,	// F
        -0.5f, -0.5f, -0.5f,0.0f, 0.0f,	// E

        // 左侧面
        -0.5f, 0.5f, 0.5f,0.0f, 1.0f,	// D
        -0.5f, 0.5f, -0.5f,1.0, 1.0f,   // H
        -0.5f, -0.5f, -0.5f,1.0f, 0.0f,	// E
        -0.5f, -0.5f, -0.5f,1.0f, 0.0f,	// E
        -0.5f, -0.5f, 0.5f,0.0f, 0.0f,	// A
        -0.5f, 0.5f, 0.5f,0.0f, 1.0f,	// D

        // 右侧面
        0.5f, -0.5f, -0.5f,1.0f, 0.0f,	// F
        0.5f, 0.5f, -0.5f,1.0f, 1.0f,	// G
        0.5f, 0.5f, 0.5f,0.0f, 1.0f,	// C
        0.5f, 0.5f, 0.5f,0.0f, 1.0f,	// C
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	// B
        0.5f, -0.5f, -0.5f,1.0f, 0.0f,	// F

        // 顶面
        0.5f, 0.5f, -0.5f,1.0f, 1.0f,	// G
        -0.5f, 0.5f, -0.5f,0.0, 1.0f,   // H
        -0.5f, 0.5f, 0.5f,0.0f, 0.0f,	// D
        -0.5f, 0.5f, 0.5f,0.0f, 0.0f,	// D
        0.5f, 0.5f, 0.5f,1.0f, 0.0f,	// C
        0.5f, 0.5f, -0.5f,1.0f, 1.0f,	// G

        // 底面
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
	// 用于绘制FBO纹理的矩形顶点属性数据
	GLfloat quadVertices[] = {
		// 位置 纹理坐标
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

    GLuint cubeVaoId, cubeVboId;
    glGenVertexArrays(1, &cubeVaoId);
    glBindVertexArray(cubeVaoId);
    glGenBuffers(1, &cubeVboId);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
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

	GLuint quadVaoId, quadVboId;
	glGenVertexArrays(1, &quadVaoId);
	glBindVertexArray(quadVaoId);
	glGenBuffers(1, &quadVboId);
	glBindBuffer(GL_ARRAY_BUFFER, quadVboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid *)(2 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    GLuint cubeTexId = TextureHelper::load2DTexture("../resources/textures/container.jpg");
	GLuint planeTexId = TextureHelper::load2DTexture("../resources/textures/metal.png");

    Shader shader("scene.vert", "scene.frag");
	Shader quadShader("quad.vert", "quad.frag");

	GLuint fboId, colorTexId, depthStencilTexId;
	//if (!prepareFBO1(colorTexId, depthStencilTexId, fboId)) {
	if (!prepareFBO1(colorTexId, fboId)) {
		std::cout << "Error::FBO :" << " not complete." << std::endl;
		glfwTerminate();
		std::system("pause");
		return -1;
	}

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 使用线框模式绘制

    while (!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = (GLfloat)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents(); // 处理例如鼠标 键盘等事件
        movement(); // 根据用户操作情况 更新相机属性

        shader.use();

        glm::mat4 projection = glm::perspective(camera.mouseZoom, (GLfloat)WINDOW_SIZE / WINDOW_SIZE, 1.0f, 10.0f);
        glm::mat4 view = camera.getViewMatrix();

        glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.programId, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 model;

		// 启用用户自定义的FBO
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		// 清除颜色缓冲区 重置为指定颜色
		glClearColor(0.18, 0.04, 0.14, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(cubeVaoId);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexId);
		// 绘制第一个立方体
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// 绘制第二个立方体
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
		// 绘制平面
		glBindVertexArray(planeVaoId);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planeTexId);
		model = glm::mat4();
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// 恢复默认FBO	在矩形上绘制FBO的纹理
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		quadShader.use();
		glBindVertexArray(quadVaoId);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexId);
		glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
    }
	glDeleteVertexArrays(1, &cubeVaoId);
	glDeleteVertexArrays(1, &planeVaoId);
	glDeleteVertexArrays(1, &quadVaoId);
	glDeleteBuffers(1, &cubeVboId);
	glDeleteBuffers(1, &planeVboId);
	glDeleteBuffers(1, &quadVboId);
	glDeleteFramebuffers(1, &fboId);

    glfwTerminate();

    return 0;
}
