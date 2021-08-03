#version 430
layout(location=0) in vec3 VertexPosition;//节点位置
layout(location=1) in vec3 VertexNormal;//节点法向

out vec3 LightIntensity;//输出光强
uniform vec4 LightPosition;//灯光位置
uniform vec3 Kd;//漫反射系数
uniform vec3 Ld;//入射光

uniform mat4 ModelViewMatrix;//模型视角矩阵
uniform mat3 NormalMatrix;//归一化矩阵
uniform mat4 ProjectionMatrix;//投影矩阵
uniform mat4 MVP;//模型视角投影矩阵
void main(void)
{
    vec3 tnorm = normalize( NormalMatrix * VertexNormal);//获取节点法向的归一化值
    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition,1.0);//获取可视的位置
    vec3 s = normalize(vec3(LightPosition - eyeCoords));//入射光强

    LightIntensity = Ld * Kd * max( dot( s, tnorm ), 0.0 );//phong式光照明模型

    gl_Position = MVP * vec4(VertexPosition,1.0);//变换节点


}
