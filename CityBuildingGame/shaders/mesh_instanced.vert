#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 aModel;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
	FragPos = vec3(aModel * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(aModel))) * aNormal;
	TexCoords = aTexCoord;
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

	gl_Position = projection * view * aModel * vec4(aPos, 1.0f);
}