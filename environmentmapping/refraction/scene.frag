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

	float indexRation = 1.00 / 1.52;
	vec3 viewDirection = normalize(fragPosition - viewPosition);// 注意这里向量从观察者位置指出
	vec3 refractionDirection = refract(viewDirection, normal, indexRation);
	
	finalColor = texture(tex, refractionDirection);// 使用 折射向量 采样环境纹理
}