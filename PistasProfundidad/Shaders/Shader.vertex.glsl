#version 400

layout(location=0) in vec3 in_Position;
layout(location=1) in vec4 in_Color;

uniform mat4 projectionMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 modelMatrix;  

out vec4 frag_Color;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position,1.0);
	frag_Color = in_Color;
}