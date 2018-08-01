#version 330

uniform samplerCube tex;
uniform mat4 model;
uniform vec3 viewPosition;

in vec3 fragVert;
in vec3 fragNormal;

out vec4 finalColor;


void main()
{
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normalMatrix * fragNormal);

	vec3 fragPosition = vec3(model * vec4(fragVert, 1.0));

	vec3 viewDirection = normalize(fragPosition - viewPosition);
	vec3 reflectionDirection = reflect(viewDirection, normal);
	
	finalColor = texture(tex, reflectionDirection);
}