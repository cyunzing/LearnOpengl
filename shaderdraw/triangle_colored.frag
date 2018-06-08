#version 330

in vec3 cl;
out vec4 color;

void main()
{
	color = vec4(cl, 1.0);
}