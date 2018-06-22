#version 330

uniform sampler2D tex;

in vec2 fragTexCoord;

out vec4 finalColor;


void main()
{
	vec4 color = texture(tex, fragTexCoord);
	if (color.a < 0.1)// 根据alpha值 丢弃片元 < 0.1表示接近完全透明
		discard;
	finalColor = color;
}