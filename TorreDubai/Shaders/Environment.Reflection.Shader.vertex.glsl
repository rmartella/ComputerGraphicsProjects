#version 400

layout (location = 0) in vec3 in_Position; // positions from mesh
layout (location = 3) in vec3 Normal; // normals from mesh

uniform mat4 projectionMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 modelMatrix;

out vec3 pos_eye;
out vec3 n_eye;

void main (void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1);
  	pos_eye = vec3 (viewMatrix * modelMatrix * vec4 (in_Position, 1.0));
 	n_eye = vec3 (viewMatrix * modelMatrix * vec4 (Normal, 0.0));
}