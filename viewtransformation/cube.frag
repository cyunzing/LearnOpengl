#version 330

in vec3 VertColor;
in vec2 TexCoord;

uniform sampler2D tex;

out vec4 color;


void main()
{
	color = texture(tex, vec2(TexCoord.s, 1.0 - TexCoord.t));
}