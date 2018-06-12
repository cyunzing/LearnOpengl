#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../common/shader.h"
#include "../common/texture.h"

#define WINDOW_SIZE 500
#define MULTI_CUBE 1
#define MULTI_VIEW 1

glm::mat4 computeLookAtMatrix(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up)
{
	glm::vec3 f = glm::normalize(center - eye);
	glm::vec3 s = glm::normalize(glm::cross(f, up));
	glm::vec3 u = glm::normalize(glm::cross(s, f));

	glm::mat4 lookAtMat(
		glm::vec4(s.x, u.x, -f.x, 0.0),
		glm::vec4(s.y, u.y, -f.y, 0.0),
		glm::vec4(s.z, u.z, -f.z, 0.0),
		glm::vec4(-glm::dot(s, eye), -glm::dot(u, eye), glm::dot(f, eye), 1.0)
		);
	return lookAtMat;
}

glm::mat4 computeLookAtMatrix2(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up)
{
	glm::vec3 f = glm::normalize(center - eye);
	glm::vec3 s = glm::normalize(glm::cross(f, up));
	glm::vec3 u = glm::normalize(glm::cross(s, f));

	glm::mat4 rotate(
		glm::vec4(s.x, u.x, -f.x, 0.0),
		glm::vec4(s.y, u.y, -f.y, 0.0),
		glm::vec4(s.z, u.z, -f.z, 0.0),
		glm::vec4(0.0, 0.0, 0.0, 1.0)
	);
	glm::mat4 translate;
	translate = glm::translate(translate, -eye);
	return rotate * translate;
}

// xoz平面内圆形坐标
glm::vec3 getEyePosCircle()
{
	GLfloat radius = 6.0f;
	GLfloat xPos = radius * cos(glfwGetTime());
	GLfloat zPos = radius * sin(glfwGetTime());
	return glm::vec3(xPos, 0.0f, zPos);
}

glm::vec3 getEyePosSphere()
{
	GLfloat radius = 6.0f;
	GLfloat theta = glfwGetTime(), phi = glfwGetTime() / 2.0f;
	GLfloat xPos = radius * sin(theta) * cos(phi);
	GLfloat yPos = radius * sin(theta) * sin(phi);
	GLfloat zPos = radius * cos(theta);
	return glm::vec3(xPos, yPos, zPos);
}

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

	GLFWwindow *window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "View Transformation", NULL, NULL);
	if (!window)
		return -1;

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyboard);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		return -1;

	glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);

	// 指定顶点属性数据 顶点位置 颜色 纹理
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// A
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// B
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,	    // C
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,	    // C
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// D
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// A


		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// E
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0, 1.0f,    // H
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// G
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// G
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// F
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// E

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// D
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0, 1.0f,    // H
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,	// E
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,	// E
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// A
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// D

		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// F
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// G
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,	    // C
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,	    // C
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	// B
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// F

		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// G
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0, 1.0f,    // H
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// D
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// D
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,	// C
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// G

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// A
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,	// E
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,	// F
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,	// F
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// B
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// A
	};

	GLuint vaoId, vboId;
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid *)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader shader("cube.vert", "cube.frag");

	GLuint tId = TextureHelper::load2DTexture("./resources/textures/cat.png");

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vaoId);
		shader.use();

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)(WINDOW_SIZE) / WINDOW_SIZE, 1.0f, 100.0f);
#if MULTI_VIEW
		glm::vec3 eye = getEyePosSphere();
#else
		glm::vec3 eye = getEyePosCircle();
#endif
		glm::mat4 view = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 model;

		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "view"), 1, GL_FALSE, glm::value_ptr(view));
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tId);
		glUniform1i(glGetUniformLocation(shader.programId, "tex"), 0);

#if MULTI_CUBE
		// 指定立方体位移
		glm::vec3 cubePostitions[] = {
			glm::vec3(0.0f, 0.0f, 1.2f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(1.2f, 1.2f, 0.0f),
			glm::vec3(-1.2f, 1.2f, 0.0f),
			glm::vec3(-1.2f, -1.5f, 0.0f),
			glm::vec3(1.2f, -1.5f, 0.0f),
			glm::vec3(0.0f, 0.0f, -1.2f),
		};
		// 在主循环中绘制立方体
		for (int i = 0; i < sizeof(cubePostitions) / sizeof(cubePostitions[0]); ++i) {
			model = glm::mat4();
			model = glm::translate(model, cubePostitions[i]);
			glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
#else
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
#endif

		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &vaoId);
	glDeleteBuffers(1, &vboId);

	glfwTerminate();

    return 0;
}
