#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../common/shader.h"
#include "../common/texture.h"

#define WINDOW_SIZE 500
#define MODEL_TRANSLATE 1
#define MODEL_SCALE 0
#define MODEL_ROTATE 0
#define MODEL_REFLECT 0

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

    GLFWwindow *window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "Transformation", NULL, NULL);
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
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 0
        0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 1
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // 2
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 3
    };
    GLushort indices[] = {
        0, 1, 2,  // 第一个三角形
        0, 2, 3   // 第二个三角形
    };

	// 指定坐标轴三角形箭头属性数据 顶点 颜色
	GLfloat axisTriangleData[] = {
		0.945f,    0.03125f,  0.0f,   1.0f, 0.0f, 0.0f, // +x轴三角形
		1.0f,      0.0f,      0.0f,   1.0f, 0.0f, 0.0f,
		0.945f,    -0.03125f, 0.0f,   1.0f, 0.0f, 0.0f,
		-0.03125f, 0.945f,    0.0f,   0.0f, 1.0f, 0.0f,// +y轴三角形
		0.0f,      1.0f,      0.0f,   0.0f, 1.0f, 0.0f,
		0.03125f,  0.945f,    0.0f,   0.0f, 1.0f, 0.0f,
		-0.03125f, 0.0f,      0.945f, 0.0f, 0.0f, 1.0f,// +z轴三角形
		0.0f,      0.0f,      1.0f,   0.0f, 0.0f, 1.0f,
		0.03125f,  0.0f,      0.945f, 0.0f, 0.0f, 1.0f,
	};
	// 指定坐标轴直线属性数据 顶点 颜色
	GLfloat axisLineData[] = {
		-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f

	};
	GLuint vaoId, vboId, eboId;

	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid *)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLuint axisVAOIds[2], axisVBOIds[2];
	glGenVertexArrays(2, axisVAOIds);
	glGenBuffers(2, axisVBOIds);

	glBindVertexArray(axisVAOIds[0]);
	glBindBuffer(GL_ARRAY_BUFFER, axisVBOIds[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axisTriangleData), axisTriangleData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(axisVAOIds[1]);
	glBindBuffer(GL_ARRAY_BUFFER, axisVBOIds[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axisLineData), axisLineData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader shader("rectangle.vert", "rectangle.frag");
	Shader axiShader("axis.vert", "axis.frag");

    GLuint tId = TextureHelper::load2DTexture("../resources/textures/cat.png");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vaoId);
		shader.use();

		glm::mat4 projection;// 投影变换矩阵
		glm::mat4 view;// 视变换矩阵
		glm::mat4 model;// 模型变换矩阵

		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// 启用纹理单元 绑定纹理对象
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tId);
		glUniform1i(glGetUniformLocation(shader.programId, "tex"), 0);// 设置纹理单元为0号
		
		// 绘制第一个矩形
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

		// 绘制第二个矩形
		model = glm::mat4();
#if MODEL_TRANSLATE
		model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
#elif MODEL_SCALE
		model = glm::translate(model, glm::vec3(-0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
#elif MODEL_ROTATE
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
#elif MODEL_REFLECT
		model = glm::scale(model, glm::vec3(-1.0f, 1.0f, 1.0f));
#endif
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

		// 绘制第三个矩形
		model = glm::mat4();
#if MODEL_TRANSLATE
		model = glm::translate(model, glm::vec3(-0.8f, -0.8f, 0.0f));
#elif MODEL_SCALE
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
#elif MODEL_ROTATE
		// 平移至第三象限
		model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
		// 下面为绕着中心旋转的三个矩阵
		model = glm::translate(model, glm::vec3(0.25f, 0.25f, 0.0f));
		model = glm::rotate(model, (GLfloat)glfwGetTime() * 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.25f, -0.25f, 0.0f));
#elif MODEL_REFLECT
		model = glm::scale(model, glm::vec3(-1.0f, -1.0f, 1.0f));
#endif
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

		// 绘制第四个矩形
		model = glm::mat4();
#if MODEL_TRANSLATE
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
#elif MODEL_SCALE
		model = glm::translate(model, glm::vec3(0.0f, -0.25f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 0.5f, 1.0f));
#elif MODEL_ROTATE
		// 绘制第四个矩形 绕着右下角旋转
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, (GLfloat)glfwGetTime() * 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
#elif MODEL_REFLECT
		model = glm::scale(model, glm::vec3(1.0f, -1.0f, 1.0f));
#endif
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

		// 绘制坐标轴
		glBindVertexArray(axisVAOIds[0]);
		axiShader.use();
		glDrawArrays(GL_TRIANGLES, 0, 9);

		glBindVertexArray(axisVAOIds[1]);
		glDrawArrays(GL_LINES, 0, 6);

		glBindVertexArray(0);
		glUseProgram(0);

        glfwSwapBuffers(window);
    }

	glDeleteVertexArrays(1, &vaoId);
	glDeleteBuffers(1, &vboId);

    glfwTerminate();

    return 0;
}
