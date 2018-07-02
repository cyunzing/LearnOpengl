#define GLEW_STATIC
#include "gl/glew.h"
#include "GLFW/glfw3.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../../common/camera.h"
#include "../../common/skybox.h"

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

    GLFWwindow *window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "Skybox optimized", NULL, NULL);
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

    GLuint cubeTexId = TextureHelper::load2DTexture("../../resources/textures/container.jpg");

	Shader shader("scene.vert", "scene.frag");
	Shader skyboxShader("skybox.vert", "skybox.frag");

    std::vector<std::string> faces;
#if 0
    faces.push_back("../../resources/skyboxes/sky/sky_rt.jpg");
    faces.push_back("../../resources/skyboxes/sky/sky_lf.jpg");
    faces.push_back("../../resources/skyboxes/sky/sky_up.jpg");
    faces.push_back("../../resources/skyboxes/sky/sky_dn.jpg");
    faces.push_back("../../resources/skyboxes/sky/sky_bk.jpg");
    faces.push_back("../../resources/skyboxes/sky/sky_ft.jpg");
#else
    faces.push_back("../../resources/skyboxes/urbansp/urbansp_rt.tga");
    faces.push_back("../../resources/skyboxes/urbansp/urbansp_lf.tga");
    faces.push_back("../../resources/skyboxes/urbansp/urbansp_up.tga");
    faces.push_back("../../resources/skyboxes/urbansp/urbansp_dn.tga");
    faces.push_back("../../resources/skyboxes/urbansp/urbansp_bk.tga");
    faces.push_back("../../resources/skyboxes/urbansp/urbansp_ft.tga");
#endif
	SkyBox skybox;
	skybox.init(faces);

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
        glm::mat4 model = glm::mat4();
        glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.programId, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(cubeVaoId);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexId);
        glUniform1i(glGetUniformLocation(shader.programId, "tex"), 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 然后绘制包围盒
		skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.getViewMatrix()));// 视变换矩阵 移除translate部分
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
    glDeleteVertexArrays(1, &cubeVaoId);
    glDeleteBuffers(1, &cubeVboId);

    glfwTerminate();
    return 0;
}
