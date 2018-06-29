#version 330

layout(location = 0) in vec2 vert;
layout(location = 1) in vec2 vertTexCoord;

out vec2 fragTexCoord;


void main()
{
	gl_Position =  vec4(vert.x, vert.y, 0.0, 1.0);
	fragTexCoord = vertTexCoord;
}