#version 330

uniform sampler2D tex;
uniform float mixValue;

in vec3 fragColor;
in vec2 fragTexCoord;

out vec4 finalColor;


void main()
{
	finalColor = mix(texture(tex, fragTexCoord), vec4(fragColor, 1.0f), mixValue);
}