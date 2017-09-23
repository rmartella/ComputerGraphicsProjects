#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 LightDirection_cameraspace_2;
in vec3 Position_Move;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;
uniform vec3 LightPosition_worldspace_2;
uniform vec3 LightPosition_SpotDirection;

void main(){

	vec3 color_1, color_2;

	vec3 LightColor = vec3(1,1,1);
	vec3 LightColor_2 = vec3(1,0,0);
	vec3 spec = vec3(0.0);
	vec3 spec_spot = vec3(0.0);
	float shininess = 1000.0;
	float shininess_spot = 1000.0;

	vec3 MaterialDiffuseColor = texture2D( myTextureSampler, UV ).rgb;
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	//Point Lights 
	vec3 n = normalize(Normal_cameraspace);
	vec3 l = normalize(LightDirection_cameraspace);
	vec3 e = normalize(EyeDirection_cameraspace);

	float intensity = max(dot(n,l), 0.0);
	if (intensity > 0.0) {
		vec3 h = normalize(l + e);
		float intSpec = max(dot(h,n), 0.0);
		spec = MaterialSpecularColor * pow(intSpec, shininess);
	}

	//Spotlights
	float intensity_spot = 0.0;
	float l_spotCutOff = 0.999;

	vec3 ld = normalize(LightDirection_cameraspace_2);
	vec3 sd = normalize(LightPosition_SpotDirection); //direccion del spotlight
	// inside the cone?
	if (dot(sd,ld) > l_spotCutOff) {
		vec3 n2 = normalize(Normal_cameraspace);
		intensity_spot = max(dot(n2,ld), 0.0);
		if (intensity_spot > 0.0) {
			vec3 ed = normalize(EyeDirection_cameraspace);
			vec3 hd = normalize(ld + ed);
			float intSpec_spot = max(dot(hd,n2), 0.0);
			spec_spot = MaterialSpecularColor * pow(intSpec_spot, shininess_spot);
		}
	}

	color_1 = max((intensity * MaterialDiffuseColor + spec) * LightColor, MaterialAmbientColor);
	color_2 = max((intensity_spot * MaterialDiffuseColor + spec_spot) * LightColor_2, MaterialAmbientColor);
	color =  color_1 + color_2;


/*
	// Light emission properties
	// You probably want to put them as uniforms
	vec3 LightColor = vec3(1,1,1);
	//float LightPower = 25.0f;
	vec3 LightColor_2 = vec3(1,0,0);
	//float LightPower_2 = 25.0f;
	float LightPower = 25.0f * Position_Move.z;
	float LightPower_2 = 25.0f * Position_Move.z;

	// Material properties
	//vec3 MaterialDiffuseColor = texture2D( myTextureSampler, UV ).rgb;
	vec3 MaterialDiffuseColor = vec3(0.999,0.999,0.999);
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	// Distance to the light
	//float distance = (length( LightPosition_worldspace - Position_worldspace ));
	//float distance_2 = (length( LightPosition_worldspace_2 - Position_worldspace));
	float distance = (length( LightPosition_worldspace - Position_Move));
	float distance_2 = (length( LightPosition_worldspace_2 - Position_Move));

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirection_cameraspace );
	vec3 l2 = normalize( LightDirection_cameraspace_2 );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );
	float cosTheta_2 = clamp( dot( n,l2 ), 0,1 );
	
	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	vec3 R_2 = reflect(-l2,n);
	//vec3 R_2 = reflect(-l2,-n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	float cosAlpha_2 = clamp( dot( E,R_2 ), 0,1 );
	
	//color = 
		  // Ambient : simulates indirect lighting
		//MaterialAmbientColor +
		  // Diffuse : "color" of the object
		//MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		  // Specular : reflective highlight, like a mirror
		//MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);

		//color =	//(MaterialAmbientColor / (distance*distance)) + 
			//	MaterialAmbientColor +
			//	MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) + 
			//	MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,500) / (distance*distance);

	color = 
		MaterialAmbientColor +
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		MaterialDiffuseColor * LightColor_2 * LightPower_2 * cosTheta_2 / (distance_2 * distance_2) +
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance * distance) +
		MaterialSpecularColor * LightColor_2 * LightPower_2 * pow(cosAlpha_2,5) / (distance_2 * distance_2);

	//Normalizar colores = gris
	//vec3 colorN = normalize(color);
	//color = color/colorN;

	//Normalizar colores corresctamente
	//float MaxColor = max(max(color.x, color.y), color.z);
	//color = color/MaxColor;

*/
}