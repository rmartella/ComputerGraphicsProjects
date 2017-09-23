#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 in_Position;                                             
                                                                                    
uniform mat4 projectionMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 modelMatrix;                                                                
                                                                                    
out vec3 TexCoord0;                                                                 
                                                                                    
void main()                                                                         
{                                                                                   
    vec4 WVP_Pos = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);                                      
    gl_Position = WVP_Pos.xyww;                                                     
    TexCoord0   = in_Position;                                                         
}
