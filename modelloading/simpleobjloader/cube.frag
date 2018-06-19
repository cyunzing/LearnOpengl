#version 330

uniform sampler2D tex;

in vec2 fragTexCoord;
in vec3 fragNormal;

out vec4 finalColor;

void main()
{
	finalColor = texture(tex, fragTexCoord);	
}