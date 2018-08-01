#version 330

layout(location = 0) in vec3 vert;
layout(location = 1) in vec3 vertColor;
layout(location = 2) in vec2 vertTexCoord;

out vec3 fragColor;
out vec2 fragTexCoord;


void main()
{
	gl_Position = vec4(vert, 1.0);
	fragColor = vertColor;
	fragTexCoord = vertTexCoord;
}