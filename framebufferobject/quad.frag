#version 330

uniform sampler2D tex;

in vec2 fragTexCoord;

out vec4 finalColor;

vec3 inverse()
{
	return vec3(1.0 - texture(tex, fragTexCoord));
}

vec3 grayscale()
{
	vec4 color = texture(tex, fragTexCoord);
	float gray = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b; // 取权重计算
	return vec3(gray, gray, gray);
}

// sharpen效果kernel
float sharpenKernel[9] = float[](
	-1.0, -1.0, -1.0,
	-1.0, 9.0, -1.0,
	-1.0, -1.0, -1.0
);

// 模糊效果kernel
float blurKernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);

// edge-detection效果kernel
float edgeDetectionKernel[9] = float[](
	1.0, 1.0, 1.0,
	1.0, -8.0, 1.0,
	1.0, 1.0, 1.0
);

// 浮雕效果
float embossKernel[9] = float[](
	-2.0, -1.0, 0.0,
	-1.0, 1.0, 1.0,
	0.0, 1.0, 2.0
);

const float offset = 1.0 / 300;  // 9个位置的纹理坐标偏移量
vec3 kernelEffect(float kernel[9])  // 利用Kernel矩阵计算效果
{
   // 确定9个位置的偏移量
   vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top-left 左上方
        vec2(0.0f,    offset),  // top-center 正上方
        vec2(offset,  offset),  // top-right 右上方
        vec2(-offset, 0.0f),    // center-left 中间左边
        vec2(0.0f,    0.0f),    // center-center 正中位置
        vec2(offset,  0.0f),    // center-right 中间右边
        vec2(-offset, -offset), // bottom-left 底部左边
        vec2(0.0f,    -offset), // bottom-center 底部中间
        vec2(offset,  -offset)  // bottom-right  底部右边
    );
	// 计算9个位置的纹理
	vec3 sampleText[9];
	for(int i = 0; i < 9; ++i) {
		sampleText[i] = vec3(texture(tex, fragTexCoord.st + offsets[i]));
	}
	// 利用权值求最终纹理颜色
	vec3 result = vec3(0.0);
	for(int i = 0; i < 9; ++i) {
		result += sampleText[i] * kernel[i];
	}
	return result;
}

void main()
{
	//finalColor = texture(tex, fragTexCoord);

	//finalColor = vec4(inverse(), 1.0);
	//finalColor = vec4(grayscale(), 1.0);
	//finalColor = vec4(kernelEffect(sharpenKernel), 1.0);
	//finalColor = vec4(kernelEffect(blurKernel), 1.0);
	//finalColor = vec4(kernelEffect(edgeDetectionKernel), 1.0);
	finalColor = vec4(kernelEffect(embossKernel), 1.0);
	//finalColor = vec4(kernelEffect(blurKernel), 1.0);
}