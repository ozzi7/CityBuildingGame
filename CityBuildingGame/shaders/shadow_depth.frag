#version 450 core
struct Material {
    sampler2D diffuse;
    //vec3 specular;    
    //float shininess;
}; 

uniform Material material;
in vec2 TexCoords;

void main()
{   
	vec4 texColor = texture(material.diffuse, TexCoords);
	if(texColor.a < 0.9)
		discard;
}  