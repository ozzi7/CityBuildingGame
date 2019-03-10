#version 450 core
in vec2 TexCoords;

out vec4 FragColor;

uniform vec4 RTPixelSizePixelSizeHalf;
uniform sampler2D Tex0;

 vec3 GaussianBlur( sampler2D tex0, vec2 centreUV, vec2 halfPixelOffset, vec2 pixelOffset )                                                                           
 {                                                                                                                                                                    
     vec3 colOut = vec3( 0, 0, 0 );                                                                                                                                   
                                                                                                                                                                      
     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
     // Kernel width 35 x 35
     //
     const int stepCount = 3;
     //
     const float gWeights[stepCount] ={
        0.27901,
        0.44198,
		0.27901
     };
     const float gOffsets[stepCount] ={
        0.66293,
        2.47904,
        4.46232
     };
     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
                                                                                                                                                                      
     for( int i = 0; i < stepCount; i++ )                                                                                                                             
     {                                                                                                                                                                
         vec2 texCoordOffset = gOffsets[i] * pixelOffset;                                                                                                           
         vec3 col = texture( tex0, centreUV + texCoordOffset ).xyz + texture( tex0, centreUV - texCoordOffset ).xyz;                                                
         colOut += gWeights[i] * col;                                                                                                                               
     }                                                                                                                                                                
                                                                                                                                                                      
     return colOut;                                                                                                                                                   
 }     

void main()
{
    FragColor.xyz = GaussianBlur( Tex0, TexCoords, vec2( RTPixelSizePixelSizeHalf.z, 0 ), vec2( RTPixelSizePixelSizeHalf.x, 0 ) );
    FragColor.w = 0.05;
}