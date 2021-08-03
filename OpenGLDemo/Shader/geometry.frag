#version 410
in vec3 LightIntensity;//输入灯光强度
layout(location=0) out vec4 FragColor;

void main(void)
{
    FragColor= vec4(LightIntensity,1.0);
}
