#version 330

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPosition;

layout(location = 0) in vec3 vert;
layout(location = 1) in vec2 vertTexCoord;
layout(location = 2) in vec3 vertNormal;

out vec4 fragColor;

void main()
{
	//calculate normal in world coordinates
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normalMatrix * vertNormal);

	//calculate the location of this fragment (pixel) in world coordinates
	vec3 vertPosition = vec3(model * vec4(vert, 1.0));

	// 环境光成分
	float ambientStrenght = 0.1f;
	vec3 ambient = ambientStrenght * lightColor;

	// 漫反射光成分 此时需要光线方向为指向光源
	vec3 lightDirection = normalize(lightPosition - vertPosition);
	float brightness = max(dot(lightDirection, normal), 0.0);
	vec3 diffuse = brightness * lightColor;

	// 镜面反射成分 此时需要光线方向为由光源指出
	float specularStrenght = 0.5f;
	vec3 reflectDirection = normalize(reflect(-lightDirection, normal));
	vec3 viewDirection = normalize(viewPosition - vertPosition);
	float specFactor = pow(max(dot(reflectDirection, viewDirection), 0.0), 32);// 32为镜面高光系数
	vec3 specular = specularStrenght * specFactor * lightColor;

	fragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
	
	gl_Position = projection * view * model * vec4(vert, 1.0);
}