#ifndef CAMERA_H
#define CAMERA_H

#include "gl/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// 定义移动方向
enum CameraMovement {
	Forward, Backward, Left, Right
};

// 定义预设常量
const GLfloat YAW = 0.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat MOUSE_SENSITIVITY = 0.05f;
const GLfloat MOUSE_ZOOM = 45.0f;
const GLfloat MAX_PITCH_ANGLE = 89.0f;// 防止万向锁


class Camera
{
public:
	Camera(glm::vec3 pos = glm::vec3(0.0, 0.0, 2.0), 
		glm::vec3 up = glm::vec3(0.0, 1.0, 0.0),
		GLfloat yaw = YAW,
		GLfloat pitch = PITCH)
		: position(pos), forward(0.0, 0.0, -1.0), viewUp(up),
		moveSpeed(SPEED), mouseZoom(MOUSE_ZOOM), mouseSensitivity(MOUSE_SENSITIVITY),
		yawAngle(yaw), pitchAngle(pitch) 
	{
		updateCameraVectors();
	}

	~Camera() {}

	// 获取视变换矩阵
	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(position, position + forward, viewUp);
	}

	// 处理键盘按键后方向移动
	void handleKeyPress(CameraMovement direction, GLfloat deltaTime)
	{
		GLfloat velocity = moveSpeed * deltaTime;
		switch (direction) {
			case Forward:
				position += forward * velocity;
				break;
			case Backward:
				position -= forward * velocity;
				break;
			case Left:
				position -= side * velocity;
				break;
			case Right:
				position += side * velocity;
				break;
			default:
				break;
		}
	}

	// 处理鼠标移动
	void handleMouseMove(GLfloat xoffset, GLfloat yoffset)
	{
		xoffset *= mouseSensitivity;// 用鼠标灵敏度调节角度变换
		yoffset *= mouseSensitivity;

		pitchAngle += yoffset;
		yawAngle += xoffset;

		normalizeAngle();
		updateCameraVectors();
	}

	// 处理鼠标滚轮缩放 保持在[1.0, MOUSE_ZOOM]之间
	void handleMouseScroll(GLfloat yoffset)
	{
		if (mouseZoom >= 1.0f && mouseZoom <= MOUSE_ZOOM)
			mouseZoom -= mouseSensitivity * yoffset;
		if (mouseZoom <= 1.0f)
			mouseZoom = 1.0f;
		if (mouseZoom >= MOUSE_ZOOM)
			mouseZoom = MOUSE_ZOOM;
	}

	// 使pitch yaw角度保持在合理范围内
	void normalizeAngle()
	{
		if (pitchAngle > MAX_PITCH_ANGLE)
			pitchAngle = MAX_PITCH_ANGLE;
		if (pitchAngle < -MAX_PITCH_ANGLE)
			pitchAngle = -MAX_PITCH_ANGLE;
		if (yawAngle < 0.0f)
			yawAngle += 360.0f;
	}

	// 计算forward side向量
	void updateCameraVectors()
	{
		glm::vec3 f;
		f.x = -sin(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle));
		f.y = sin(glm::radians(pitchAngle));
		f.z = -cos(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle));
		forward = glm::normalize(f);

		glm::vec3 s;
		s.x = cos(glm::radians(yawAngle));
		s.y = 0;
		s.z = -sin(glm::radians(yawAngle));
		side = glm::normalize(s);
	}
public:
	glm::vec3 forward, viewUp, side, position;// 相机属性
	GLfloat yawAngle, pitchAngle;// 欧拉角
	GLfloat moveSpeed, mouseSensitivity, mouseZoom;// 相机选项
};

#endif // CAMERA_H
