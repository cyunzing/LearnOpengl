#version 330

uniform struct Material {
	sampler2D diffuseMap;	// 使用纹理对象根据片元位置取不同的材质属性
	sampler2D specularMap;
	float shininess; //镜面高光系数
} material;

uniform struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;	// 衰减常数
	float linear;   // 衰减一次系数
	float quadratic; // 衰减二次系数
} light;

uniform mat4 model;
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
	vec3 ambient = light.ambient * vec3(texture(material.diffuseMap, fragTexCoord));

	// 漫反射光成分 此时需要光线方向为指向光源
	vec3 lightDirection = normalize(light.position - fragPosition);
	float brightness = max(dot(lightDirection, normal), 0.0);
	vec3 diffuse = brightness * light.diffuse * vec3(texture(material.diffuseMap, fragTexCoord));

	// 镜面反射成分 此时需要光线方向为由光源指出
	vec3 reflectDirection = normalize(reflect(-lightDirection, normal));
	vec3 viewDirection = normalize(viewPosition - fragPosition);
	float specFactor = pow(max(dot(reflectDirection, viewDirection), 0.0), material.shininess);
	vec3 specular = specFactor * light.specular * vec3(texture(material.specularMap, fragTexCoord));

	// 计算衰减因子
	float distance = length(light.position - fragPosition); // 在世界坐标系中计算距离
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);
			
	finalColor = vec4((ambient + diffuse + specular) * attenuation, 1.0);
}