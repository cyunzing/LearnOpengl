#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "SOIL/SOIL.h"

#include "../common/shader.h"
#include "../common/texture.h"

#define WINDOW_SIZE 500

#define MULTI_TEXTURE

GLfloat mixValue = 0.4f;

void keyboard(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	} else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		mixValue += 0.05f;
		if (mixValue > 1.0f)
			mixValue = 1.0f;
	} else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		mixValue -= 0.05f;
		if (mixValue < 0.0f)
			mixValue = 0.0f;
	}
}

int main(int argc, char *argv[])
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "2D Textures", NULL, NULL);
	if (!window)
		return -1;

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyboard);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		return -1;

	glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);

	// Section1 准备顶点数据
	// 指定顶点属性数据 顶点位置 颜色 纹理
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f, 0.0f,  // 0
		0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,1.0f, 0.0f,  // 1
		0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f,1.0f, 1.0f,  // 2
		-0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f,0.0f, 1.0f   // 3
	};
	GLushort indices[] = {
		0, 1, 2,  // 第一个三角形
		0, 2, 3   // 第二个三角形
	};

	GLuint vaoId, vboId, eboId;
	// Step1: 创建并绑定VAO对象
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);
	// Step2: 创建并绑定VBO 对象 传送数据
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Step3: 创建并绑定EBO 对象 传送数据
	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// Step4: 指定解析方式  并启用顶点属性
	// 顶点位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// 顶点颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// 顶点纹理坐标
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // 注意不要解除EBO绑定
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Section2 准备着色器程序
	Shader shader("textures/texture.vert", 
#ifdef MULTI_TEXTURE
		"textures/texture_multi.frag"
#else
		"textures/texture.frag"
#endif
	);

#if 1
	// Section3 准备纹理对象
	GLuint tId = TextureHelper::load2DTexture("resources/textures/wood.png");
#ifdef MULTI_TEXTURE
	GLuint tId2 = TextureHelper::load2DTexture("resources/textures/cat.png");
#endif
#else
	// Section3 准备纹理对象
	// Step1 创建并绑定纹理对象
	GLuint tId;
	glGenTextures(1, &tId);
	glBindTexture(GL_TEXTURE_2D, tId);
	// Step2 设定wrap参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Step3 设定filter参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);// 为MipMap设定filter方法

	// Step4 加载纹理
	int w, h;
	GLubyte *data = SOIL_load_image("resources/textures/wood.png", &w, &h, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Step5 释放纹理图片资源
	SOIL_free_image_data(data);
	glBindTexture(GL_TEXTURE_2D, 0);
#endif

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vaoId);
		shader.use();
		// 启用纹理单元 绑定纹理对象
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tId);
		glUniform1i(glGetUniformLocation(shader.programId, "tex"), 0);// 设置纹理单元为0号

#ifdef MULTI_TEXTURE
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tId2);
		glUniform1i(glGetUniformLocation(shader.programId, "tex2"), 1);// 设置纹理单元为1号

		glUniform1f(glGetUniformLocation(shader.programId, "mixValue"), mixValue);// 设置纹理混合参数
#endif

		// 使用索引绘制
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &vaoId);
	glDeleteBuffers(1, &vboId);

	glfwTerminate();

	return 0;
}
