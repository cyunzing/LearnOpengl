#version 330

uniform mat4 model;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPosition;

in vec3 fragVert;
in vec2 fragTexCoord;
in vec3 fragNormal;

out vec4 finalColor;

void main()
{
	//calculate normal in world coordinates
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normalMatrix * fragNormal);

	//calculate the location of this fragment (pixel) in world coordinates
	vec3 fragPosition = vec3(model * vec4(fragVert, 1.0));

	// 环境光成分
	float ambientStrenght = 0.1f;
	vec3 ambient = ambientStrenght * lightColor;

	// 漫反射光成分 此时需要光线方向为指向光源
	vec3 lightDirection = normalize(lightPosition - fragPosition);
	float brightness = max(dot(lightDirection, normal), 0.0);
	vec3 diffuse = brightness * lightColor;

	// 镜面反射成分 此时需要光线方向为由光源指出
	float specularStrenght = 0.5f;
	vec3 reflectDirection = normalize(reflect(-lightDirection, normal));
	vec3 viewDirection = normalize(viewPosition - fragPosition);
	float specFactor = pow(max(dot(reflectDirection, viewDirection), 0.0), 32);// 32为镜面高光系数
	vec3 specular = specularStrenght * specFactor * lightColor;

	finalColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}