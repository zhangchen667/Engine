#version 460 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D texture_diffuse1;// 输入的屏幕纹理
uniform vec2 sampleOffset;// 屏幕像素大小的倒数（1/宽度, 1/高度）,用于计算相邻像素坐标,就是分辨率的倒数
// FXAA 参数（可调整）
#define FXAA_SPAN_MAX 4.0// 最大采样跨度
#define FXAA_REDUCE_MUL 1.0/8.0     // 降低阈值的乘数
#define FXAA_REDUCE_MIN 1.0/128.0   // 最小降低阈值

// 计算像素亮度（人眼对亮度的感知权重：R*0.299 + G*0.587 + B*0.114）
float luma(vec3 color) {
    return dot(color, vec3(0.299, 0.587, 0.114));
}


void main() {
    
    // 1. 采样当前像素及周围4个像素（上下左右）
    vec3 rgbM = texture(texture_diffuse1, TexCoords).rgb;
    vec3 rgbN = texture(texture_diffuse1, TexCoords + vec2(0.0, -sampleOffset.y)).rgb;
    vec3 rgbS = texture(texture_diffuse1, TexCoords + vec2(0.0, sampleOffset.y)).rgb;
    vec3 rgbE = texture(texture_diffuse1, TexCoords + vec2(sampleOffset.x, 0.0)).rgb;
    vec3 rgbW = texture(texture_diffuse1, TexCoords + vec2(-sampleOffset.x, 0.0)).rgb;

    // 2. 计算亮度，检测边缘
    float lumaM = luma(rgbM);// 中心像素亮度
    float lumaN = luma(rgbN);// 上方像素亮度
    float lumaS = luma(rgbS);
    float lumaE = luma(rgbE);
    float lumaW = luma(rgbW);

    // 边缘亮度极值
    float lumaMin = min(lumaM, min(min(lumaN, lumaS), min(lumaE, lumaW)));// 用于检测边缘
    float lumaMax = max(lumaM, max(max(lumaN, lumaS), max(lumaE, lumaW)));
    float lumaRange = lumaMax - lumaMin;

    // 无边缘：直接返回原颜色
    if (lumaRange < max(FXAA_REDUCE_MIN, lumaMax * FXAA_REDUCE_MUL)) {
        FragColor = vec4(rgbM, 1.0);
        return;
    }

    // 3. 检测边缘方向（水平/垂直）
    vec2 dir;
    dir.x = -((lumaE - lumaW) + (lumaE - lumaW));// 水平方向梯度亮度差
    dir.y = -((lumaN - lumaS) + (lumaN - lumaS));//垂直方向梯度亮度差

    // 归一化边缘方向
    dir = normalize(dir);

    // 4. 沿边缘方向采样，加权混合
    vec3 rgbA = 0.5 * (
        texture(texture_diffuse1, TexCoords + dir * FXAA_SPAN_MAX * sampleOffset).rgb +
        texture(texture_diffuse1, TexCoords - dir * FXAA_SPAN_MAX * sampleOffset).rgb
    );
    vec3 rgbB = rgbA * 0.5 + 0.25 * (
        texture(texture_diffuse1, TexCoords + dir * FXAA_SPAN_MAX * 0.5 * sampleOffset).rgb +
        texture(texture_diffuse1, TexCoords - dir * FXAA_SPAN_MAX * 0.5 * sampleOffset).rgb
    );

    // 5. 最终混合：避免过度模糊
    float lumaB = luma(rgbB);// 计算混合后的亮度
    if (lumaB < lumaMin || lumaB > lumaMax) {// 超出边缘亮度范围，使用较少模糊的结果
        FragColor = vec4(rgbA, 1.0);
    } else {
        FragColor = vec4(rgbB, 1.0);
    }
}
