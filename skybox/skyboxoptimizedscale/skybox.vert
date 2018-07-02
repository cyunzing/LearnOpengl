#version 330

uniform mat4 projection;
uniform mat4 view;

layout(location = 0) in vec3 vert;

out vec3 fragTexCoord;


void main()
{
	vec4 position = projection * view * model * vec4(vert, 1.0);
	gl_Position = position.xyww;// 此处让z=w 则对应的深度值变为depth = w / w = 1.0
	fragTexCoord = vert;// 当立方体中央处于原点时 立方体上位置即等价于向量
}