#version 330

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

layout(location = 0) in vec3 vert;

out vec3 fragVert;

void main()
{
	gl_Position = projection * view * model * vec4(vert, 1.0);
}