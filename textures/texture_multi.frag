#version 330

in vec3 VertColor;
in vec2 TexCoord;

uniform sampler2D tex;
uniform sampler2D tex2;
uniform float mixValue;

out vec4 color;

void main()
{
	vec4 color1 = texture(tex, TexCoord);
	//vec4 color2 = texture(tex2, TexCoord);
	vec4 color2 = texture(tex2, vec2(TexCoord.s, 1.0 - TexCoord.t));	//图片的(0,0)位置一般在左上角，而OpenGL纹理坐标的(0,0)在左下角，y轴顺序相反
																		//实现y轴的翻转
	color = mix(color1, color2, mixValue);
}