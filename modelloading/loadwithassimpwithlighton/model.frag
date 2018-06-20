#version 330

uniform struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;// 衰减常数
	float linear;// 衰减一次系数
	float quadratic;// 衰减二次系数
} light;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_specular0;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

uniform mat4 model;
uniform vec3 viewPosition;

in vec3 fragVert;
in vec2 fragTexCoord;
in vec3 fragNormal;

out vec4 finalColor;


void main()
{
	//calculate normal in world coordinates
	mat3 normalMatrix = mat3(transpose(inverse(model)));
	vec3 normal = normalize(normalMatrix * fragNormal);

	//calculate the location of this fragment (pixel) in world coordinates
	vec3 fragPosition = vec3(model * vec4(fragVert, 1.0));

	// 环境光成分
	vec3 ambient = light.ambient * vec3(texture(texture_diffuse0, fragTexCoord));

	// 漫反射光成分 此时需要光线方向为指向光源
	vec3 lightDirection = normalize(light.position - fragPosition);
	float brightness = max(dot(lightDirection, normal), 0.0);
	vec3 diffuse = brightness * light.diffuse * vec3(texture(texture_diffuse0, fragTexCoord));

	// 镜面反射成分 此时需要光线方向为由光源指出
	vec3 reflectDirection = normalize(reflect(-lightDirection, normal));
	vec3 viewDirection = normalize(viewPosition - fragPosition);
	float specFactor = pow(max(dot(reflectDirection, viewDirection), 0.0), 64.0);
	vec3 specular = specFactor * light.specular * vec3(texture(texture_specular0, fragTexCoord));

	// 计算衰减因子
	float distance = length(light.position - fragPosition);// 在世界坐标系中计算距离
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);


	finalColor = vec4((ambient + diffuse + specular), 1.0);	
}