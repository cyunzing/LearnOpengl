#define GLEW_STATIC
#include "gl/glew.h"
#include "GLFW/glfw3.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../../common/texture.h"
#include "../../common/shader.h"
#include "../../common/camera.h"
#include "../../common/skybox.h"
#include "../../common/model.h"


#define WINDOW_SIZE 500

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
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
void mousescroll(GLFWwindow *window, double xoffset, double yoffset)
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

	GLFWwindow *window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "refraction", NULL, NULL);
	if (window == NULL)
		return -1;

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyboard);
	glfwSetMouseButtonCallback(window, mousebutton);
	glfwSetCursorPosCallback(window, cursorpos);
	glfwSetScrollCallback(window, mousescroll);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		return -1;

	glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);

	Model obj;
	if (!obj.loadModel("resources/models/sphere/sphere.obj")) {
		return -1;
	}

	std::vector<std::string> faces;
	faces.push_back("resources/skyboxes/sky/sky_rt.jpg");
	faces.push_back("resources/skyboxes/sky/sky_lf.jpg");
	faces.push_back("resources/skyboxes/sky/sky_up.jpg");
	faces.push_back("resources/skyboxes/sky/sky_dn.jpg");
	faces.push_back("resources/skyboxes/sky/sky_bk.jpg");
	faces.push_back("resources/skyboxes/sky/sky_ft.jpg");
	SkyBox skybox;
	skybox.init(faces);

	Shader shader("scene.vert", "scene.frag");
	Shader skyboxShader("skybox.vert", "skybox.frag");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);

	while (!glfwWindowShouldClose(window)) {
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents(); // 处理例如鼠标 键盘等事件
		movement(); // 根据用户操作情况 更新相机属性

		glClearColor(0.18, 0.04, 0.14, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 先绘制场景
		shader.use();
		glm::mat4 projection = glm::perspective(camera.mouseZoom, (GLfloat)WINDOW_SIZE / WINDOW_SIZE, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 model;
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureId());
		glUniform1i(glGetUniformLocation(shader.programId, "tex"), 0);
		glUniform3f(glGetUniformLocation(shader.programId, "viewPosition"), camera.position.x, camera.position.y, camera.position.z);
		obj.draw(shader);

		// 然后绘制包围盒
		skyboxShader.use();
		view = glm::mat4(glm::mat3(view));// 视变换矩阵 移除translate部分
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.programId, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureId());
		glUniform1i(glGetUniformLocation(skyboxShader.programId, "skybox"), 0);
		skybox.draw(skyboxShader);

		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(window);
	}
	glfwTerminate();

	return 0;
}
