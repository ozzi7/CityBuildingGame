#version 450 core
    
uniform sampler2D Texture;

in vec2 TexCoords;

void main()
{   
	vec4 texColor = texture(Texture, TexCoords);
	if(texColor.a < 0.9)
		discard;
}  