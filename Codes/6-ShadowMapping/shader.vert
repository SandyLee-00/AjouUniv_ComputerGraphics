#version 410 core

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_TexCoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 shadowMVP;

uniform vec3 lightPos;
uniform vec3 lightInt;
uniform vec3 ambInt;
uniform vec3 color;
uniform vec3 cameraPos;

out vec3 worldPos;
out vec3 normal;
out vec2 texCoord;
out vec4 shadow_Position;

void main(void) {
    
    worldPos = (modelMat*vec4(in_Position,1)).xyz;
    normal = (modelMat*vec4(in_Normal,0)).xyz;
    texCoord = in_TexCoord;
    
    shadow_Position = shadowMVP * vec4(in_Position, 1.0);
    
    gl_Position = projMat * viewMat * modelMat * vec4(in_Position, 1.0);
    
}
