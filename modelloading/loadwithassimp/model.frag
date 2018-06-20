#version 330

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_specular0;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

in vec2 fragTexCoord;

out vec4 finalColor;


void main()
{
	finalColor = texture(texture_diffuse0, fragTexCoord);	
}