#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 aModel;

out vec2 TexCoords;

uniform mat4 lightSpaceMatrix;

void main()
{	
	TexCoords = aTexCoord;
    gl_Position = lightSpaceMatrix * aModel * vec4(aPos, 1.0);
}