#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;
layout(location = 3) in int vertexBool; 

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 LightDirection_cameraspace_2;
out vec3 Position_Move;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPosition_worldspace;
uniform vec3 LightPosition_worldspace_2;

//--------Nueva para movimiento--------
uniform vec4 MovePosition;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	if(vertexBool == 1)
	{
		gl_Position =  (MVP * (vec4(vertexPosition_modelspace,1) + MovePosition));
	}
	else
	{
		gl_Position =  (MVP * (vec4(vertexPosition_modelspace,1) + MovePosition*2));
	}
	//Position_Move = gl_Position.xyz;
	
	// Position of the vertex, in worldspace : M * position
	Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;
	Position_Move = (M * vec4(vertexPosition_modelspace,1) + MovePosition).xyz;

	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;
	vec3 LightPosition_cameraspace_2 = ( V * vec4(LightPosition_worldspace_2,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	LightDirection_cameraspace_2 = LightPosition_cameraspace_2 + EyeDirection_cameraspace;
	
	// Normal of the the vertex, in camera space
	Normal_cameraspace = ( V * M * vec4(vertexNormal_modelspace,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	//Normal_cameraspace.z = Normal_cameraspace.z * sin(gl_Position.z);

	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}

