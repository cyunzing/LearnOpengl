#version 330

uniform mat4 projection;
uniform mat4 view;

layout(location = 0) in vec3 vert;

out vec3 fragTexCoord;


void main()
{
	gl_Position = projection * view * vec4(vert, 1.0);
	fragTexCoord = vert;
}