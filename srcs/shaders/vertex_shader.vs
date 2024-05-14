#version 330 core
layout (location = 0) in vec3	aPos;
layout (location = 1) in vec3	aNormal;
layout (location = 2) in vec2	aTexCoord;
layout (location = 3) in int	aface;
layout (location = 4) in int	ablock;

out vec2		Texture;
out float		shadow;
flat out int	blockid;
flat out int	face;

uniform mat4	model;
uniform mat4	view;
uniform mat4	projection;

// struct BlockType
// {
// 	int air = 0,
// 	int grass = 1,
// 	int grassSide = 2,
// 	int snow = 3,
// 	int snowSide = 4,
// 	int water = 5,
// 	int dirt = 6,
// 	int stone = 7,
// 	int bedrock = 8,
// 	int gravel = 9,
// 	int sand = 10,
// }	type;

void main()
{
	if (ablock == 5 && aface == 1)
		gl_Position = projection * view * vec4(aPos.x, aPos.z - 0.25, aPos.y, 1.0);
	else
		gl_Position = projection * view * vec4(aPos.xzy, 1.0);
	Texture = vec2(aTexCoord.x , -aTexCoord.y);
	shadow = 1.0f;
	if (aface == 0 || aface == 4)
		shadow = 0.5f;
	blockid = ablock;
	face = aface;
}
