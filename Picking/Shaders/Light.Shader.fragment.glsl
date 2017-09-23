#version 400                                                                        
                     
const int MAX_POINT_LIGHTS = 2;        
                                                                                    
in vec2 TexCoord0;                                                                  
in vec3 Normal0;                                                                    
in vec3 WorldPos0;

uniform vec4 EyePosW;

struct Light {                                                                                           
    vec4 LightPosW; // Light's position in world space.
	vec4 LightColor; // Light's diffuse and specular contribution.    
    vec4 MaterialEmissive;
	vec4 MaterialDiffuse;
	vec4 MaterialSpecular;                                                
	float MaterialShininess;
	vec4 Ambient; // Global ambient contribution.                                                                      
};

uniform Light[MAX_POINT_LIGHTS] gLight;
uniform int gNumLights;


uniform sampler2D gSampler;
                                                         
out vec4 FragColor;                                                                 
                                                       
                                                                                    
void main()
{
    
    vec4 TotalLight = vec4(0.0);
    vec4 N = normalize( vec4(Normal0 , 1) );
    
    for (int i = 0 ; i < gNumLights ; i++) {      
		// Compute the emissive term.
    	vec4 Emissive = gLight[i].MaterialEmissive;
    	
    	// Compute the diffuse term.
    	vec4 L = normalize( gLight[i].LightPosW - vec4(WorldPos0, 1) );
    	float NdotL = max( dot( N, L ), 0 );
    	vec4 Diffuse =  NdotL * gLight[i].LightColor * gLight[i].MaterialDiffuse;
    
    	// Compute the specular term.
    	vec4 V = normalize( EyePosW - vec4(WorldPos0, 1) );
    	vec4 H = normalize( L + V );
    	vec4 R = reflect( -L, N );
    	float RdotV = max( dot( R, V ), 0 );
    	float NdotH = max( dot( N, H ), 0 );
    	vec4 Specular = pow( RdotV, gLight[i].MaterialShininess ) * gLight[i].LightColor * gLight[i].MaterialSpecular;
    	TotalLight += Emissive + gLight[i].Ambient + Diffuse + Specular;
    }

	FragColor = TotalLight * texture( gSampler, TexCoord0 );
                                                                                    
}
