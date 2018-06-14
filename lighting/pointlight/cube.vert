#version 330

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

layout(location = 0) in vec3 vert;
layout(location = 1) in vec2 vertTexCoord;
layout(location = 2) in vec3 vertNormal;

out vec3 fragVert;
out vec2 fragTexCoord;
out vec3 fragNormal;

void main()
{
	fragVert = vert;
	fragTexCoord = vertTexCoord;
	fragNormal = vertNormal;

	gl_Position = projection * view * model * vec4(vert, 1.0);
}