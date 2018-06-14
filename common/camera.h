#ifndef CAMERA_H
#define CAMERA_H

#include "gl/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// �����ƶ�����
enum CameraMovement {
	Forward, Backward, Left, Right
};

// ����Ԥ�賣��
const GLfloat YAW = 0.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat MOUSE_SENSITIVITY = 0.05f;
const GLfloat MOUSE_ZOOM = 45.0f;
const GLfloat MAX_PITCH_ANGLE = 89.0f;// ��ֹ������


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

	// ��ȡ�ӱ任����
	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(position, position + forward, viewUp);
	}

	// ������̰��������ƶ�
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

	// ��������ƶ�
	void handleMouseMove(GLfloat xoffset, GLfloat yoffset)
	{
		xoffset *= mouseSensitivity;// ����������ȵ��ڽǶȱ任
		yoffset *= mouseSensitivity;

		pitchAngle += yoffset;
		yawAngle += xoffset;

		normalizeAngle();
		updateCameraVectors();
	}

	// �������������� ������[1.0, MOUSE_ZOOM]֮��
	void handleMouseScroll(GLfloat yoffset)
	{
		if (mouseZoom >= 1.0f && mouseZoom <= MOUSE_ZOOM)
			mouseZoom -= mouseSensitivity * yoffset;
		if (mouseZoom <= 1.0f)
			mouseZoom = 1.0f;
		if (mouseZoom >= MOUSE_ZOOM)
			mouseZoom = MOUSE_ZOOM;
	}

	// ʹpitch yaw�Ƕȱ����ں���Χ��
	void normalizeAngle()
	{
		if (pitchAngle > MAX_PITCH_ANGLE)
			pitchAngle = MAX_PITCH_ANGLE;
		if (pitchAngle < -MAX_PITCH_ANGLE)
			pitchAngle = -MAX_PITCH_ANGLE;
		if (yawAngle < 0.0f)
			yawAngle += 360.0f;
	}

	// ����forward side����
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
	glm::vec3 forward, viewUp, side, position;// �������
	GLfloat yawAngle, pitchAngle;// ŷ����
	GLfloat moveSpeed, mouseSensitivity, mouseZoom;// ���ѡ��
};

#endif // CAMERA_H
