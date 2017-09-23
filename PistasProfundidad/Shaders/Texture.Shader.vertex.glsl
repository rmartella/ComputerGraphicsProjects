#version 400

layout(location = 0) in vec3 in_Position;
layout (location = 2) in vec2 TexCoord;

uniform mat4 projectionMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 modelMatrix;
  
out vec2 TexCoord0;

out vec4 frag_Color;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1);
	TexCoord0 = TexCoord;
}