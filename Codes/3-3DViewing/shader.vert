uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 modelMat;
out vec3 posWorld;
out vec3 posCamera;
void main(void) {
    vec4 worldPos = modelMat*vec4(in_Position, 1.0);
    gl_Position= projMat*viewMat*worldPos;
    posWorld  = worldPos.xyz;
    posCamera = (viewMat*worldPos).xyz;
}
