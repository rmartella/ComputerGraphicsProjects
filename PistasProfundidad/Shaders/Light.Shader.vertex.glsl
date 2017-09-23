#version 400

layout (location = 0) in vec3 in_Position;
layout (location = 2) in vec2 TexCoord;
layout (location = 3) in vec3 Normal;

uniform mat4 projectionMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 modelMatrix;
  
out vec2 TexCoord0;
out vec3 Normal0;                                                                   
out vec3 WorldPos0;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1);
	TexCoord0 = TexCoord;
	Normal0 = (modelMatrix * vec4(Normal, 0.0)).xyz;
	WorldPos0 = (modelMatrix * vec4(in_Position, 1)).xyz;
}