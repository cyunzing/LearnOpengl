#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "../common/shader.h"
#include "../common/texture.h"

#define WINDOW_SIZE 500

GLfloat mixValue = 0.5f;


void keyboard(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
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

void prepareVertData_moreVbo(GLuint &vaoId, std::vector<GLuint> &vboIdVec)
{
	// 单个属性独立
	GLfloat vert[] = {
		-0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f,  // 顶点位置
	};
	GLfloat vertColor[] = {
		1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // 颜色
	};
	GLfloat vertTexCoord[] = {
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f     // 纹理坐标
	};

	vboIdVec.resize(3);

	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glGenBuffers(3, &vboIdVec[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vboIdVec[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboIdVec[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertColor), vertColor, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vboIdVec[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertTexCoord), vertTexCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void prepareVertData_seqBatchVbo(GLuint &vaoId, std::vector<GLuint> &vboIdVec)
{
	// 单个属性连续作为整体
	GLfloat vertices[] = {
		-0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f,  // 顶点位置
		1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // 颜色
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f     // 纹理坐标
	};

	vboIdVec.resize(1);

	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glGenBuffers(1, &vboIdVec[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vboIdVec[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(9 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(18 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void prepareVertData_interleavedBatchVbo(GLuint& vaoId, std::vector<GLuint>& vboIdVec)
{
	// 交错指定顶点属性数据
	GLfloat vertices[] = {
		// 顶点位置 颜色 纹理纹理
		-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 0
		0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 1
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // 2
	};

	vboIdVec.resize(1);

	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glGenBuffers(1, &vboIdVec[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vboIdVec[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void prepareVertData_glBufferSubData(GLuint& vaoId, std::vector<GLuint>& vboIdVec)
{
	// 单个属性独立
	GLfloat vert[] = {
		-0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f,  // 顶点位置
	};
	GLfloat vertColor[] = {
		1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // 颜色
	};
	GLfloat vertTexCoord[] = {
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f     // 纹理坐标
	};

	vboIdVec.resize(1);

	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glGenBuffers(1, &vboIdVec[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vboIdVec[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert) + sizeof(vertColor) + sizeof(vertTexCoord), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vert), vert);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vert), sizeof(vertColor), vertColor);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vert) + sizeof(vertColor), sizeof(vertTexCoord), vertTexCoord);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(9 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(18 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void prepareVertData_glMapBuffer(GLuint& vaoId, std::vector<GLuint>& vboIdVec)
{
	// 交错指定顶点属性数据
	GLfloat vertices[] = {
		// 顶点位置 颜色 纹理纹理
		-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 0
		0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 1
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // 2
	};

	vboIdVec.resize(1);

	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glGenBuffers(1, &vboIdVec[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vboIdVec[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW);// 预分配空间

	void *p = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (!p)
		return;

	memcpy(p, vertices, sizeof(vertices));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void prepareVertData_glCopyBufferSubData(GLuint& vaoId, std::vector<GLuint>& vboIdVec)
{
	// 交错指定顶点属性数据
	GLfloat vertices[] = {
		// 顶点位置 颜色 纹理纹理
		-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 0
		0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 1
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // 2
	};

	vboIdVec.resize(1);

	// Step1: 创建并绑定VAO对象
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);
	// Step2: 创建并绑定VBO 对象
	glGenBuffers(1, &vboIdVec[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vboIdVec[0]);

	GLuint tmp;
	glGenBuffers(1, &tmp);
	glBindBuffer(GL_COPY_WRITE_BUFFER, tmp);
	// Step3: 映射数据到GL_COPY_WRITE_BUFFER
	glBufferData(GL_COPY_WRITE_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW);// 预分配空间

	void *p = glMapBuffer(GL_COPY_WRITE_BUFFER, GL_WRITE_ONLY);
	if (!p)
		return;

	memcpy(p, vertices, sizeof(vertices));// 可以操作这个指针指向的内存空间
	glUnmapBuffer(GL_COPY_WRITE_BUFFER);

	// Step4: 将数据拷贝到VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW);// 注意拷贝前预分配空间
	glCopyBufferSubData(GL_COPY_WRITE_BUFFER, GL_ARRAY_BUFFER, 0, 0, sizeof(vertices));
	glDeleteBuffers(1, &tmp);

	// Step5: 指定解析方式  并启用顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


int main(int argc, char *argv[])
{
	if (glfwInit() == GLFW_FALSE)
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "buffer object", NULL, NULL);
	if (window == NULL)
		return -1;

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyboard);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		return -1;
	}

	glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);


	GLuint vaoId;
	std::vector<GLuint> vboIdVec;

	//prepareVertData_moreVbo(vaoId, vboIdVec);
	//prepareVertData_seqBatchVbo(vaoId, vboIdVec);
	//prepareVertData_interleavedBatchVbo(vaoId, vboIdVec);
	//prepareVertData_glBufferSubData(vaoId, vboIdVec);
	//prepareVertData_glMapBuffer(vaoId, vboIdVec);
	prepareVertData_glCopyBufferSubData(vaoId, vboIdVec);

	Shader shader("bufferobject/triangle.vert", "bufferobject/triangle.frag");

	GLuint texId = TextureHelper::load2DTexture("resources/textures/window.png");

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClearColor(0.18, 0.04, 0.14, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vaoId);
		shader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texId);
		glUniform1i(glGetUniformLocation(shader.programId, "tex"), 0);
		glUniform1f(glGetUniformLocation(shader.programId, "mixValue"), mixValue);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &vaoId);
	for (std::vector<GLuint>::iterator it = vboIdVec.begin(); it != vboIdVec.end(); ++it) {
		glDeleteBuffers(1, &(*it));
	}

	glfwTerminate();

	return 1;
}