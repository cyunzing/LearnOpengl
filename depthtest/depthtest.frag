#version 330

uniform sampler2D tex;

in vec2 fragTexCoord;

out vec4 finalColor;

float near = 1.0f; 
float far  = 100.0f; 
// 线性输出结果
float linearizeDepth()
{
	float zndc = gl_FragCoord.z * 2.0 - 1.0;// 计算ndc坐标 这里默认glDepthRange(0,1)
	float zeye = (2.0 * near * far) / (far + near - zndc * (far - near));// 这里分母进行了反转
	return (zeye - near) / (far - near);
}

float nonLinearDepth()
{
	float Zndc = gl_FragCoord.z * 2.0 - 1.0; // 计算ndc坐标 这里默认glDepthRange(0,1)
    float Zeye = (2.0 * near * far) / (far + near - Zndc * (far - near)); // 这里分母进行了反转
	return (1.0 / near - 1.0 / Zeye) / (1.0 / near - 1.0 / far);
}

void main()
{
	//finalColor = texture(tex, vec2(fragTexCoord.s, 1.0 - fragTexCoord.t));

	float depth = linearizeDepth();
	finalColor = vec4(vec3(depth), 1.0);
}