#version 330

uniform samplerCube skybox;

in vec3 fragTexCoord;

out vec4 finalColor;


void main()
{
	finalColor = texture(skybox, fragTexCoord);
}