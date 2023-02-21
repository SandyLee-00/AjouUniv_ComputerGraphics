#version 150 core

uniform vec3 lightPos = vec3(3,3,3);
uniform vec3 lightColor = vec3(1);
uniform vec4 diffuseMaterial =vec4(1,0.4,0,1);
uniform vec4 specularMaterial = vec4(1);
uniform vec3 ambientLight = vec3(0.1);
uniform vec3 cameraPos;
uniform float shininess;
in vec3 normal;
in vec3 worldPos;

out vec4 out_Color;

void main(void)
{
	vec3 L = normalize(lightPos);
	vec3 N = normalize(normal);
	vec3 V = normalize(cameraPos -  worldPos);
	vec3 R = N * dot(N,L)*2.-L;

	vec3 color = vec3(0);

	float diffuseFactor =clamp(dot(N,L),0,1);
	float specularFactor =pow(clamp(dot(R,V),0,1),10);
	color +=diffuseMaterial.rgb*diffuseFactor*lightColor;
	color +=specularMaterial.rgb*specularFactor*lightColor;
	color +=diffuseMaterial.rgb*ambientLight;
	
	
	
	out_Color =vec4(color,diffuseMaterial.a);
}
