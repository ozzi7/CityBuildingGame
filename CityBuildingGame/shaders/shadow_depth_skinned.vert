#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in ivec4 aBoneIDs;
layout (location = 4) in vec4 aWeights;

out vec2 TexCoords;

const int MAX_BONES = 50;

mat4 BoneTransform;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform mat4 gBones[MAX_BONES];

void main()
{
	BoneTransform = gBones[aBoneIDs[0]] * aWeights[0];
    BoneTransform += gBones[aBoneIDs[1]] * aWeights[1];
    BoneTransform += gBones[aBoneIDs[2]] * aWeights[2];
    BoneTransform += gBones[aBoneIDs[3]] * aWeights[3];

	TexCoords = aTexCoord;
    gl_Position = lightSpaceMatrix * model * BoneTransform * vec4(aPos, 1.0);
}