#version 330 core
out	vec4	FragColor;

in vec2		Texture;
in float	shadow;
flat in int	blockid;
flat in int	face;

uniform sampler2DArray	TextureArray;

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
	if (blockid == 1)
	{
		if (face == 1)
			FragColor = texture(TextureArray, vec3(Texture, 1));
		else if (face == 0)
			FragColor = texture(TextureArray, vec3(Texture, 6));
		else
			FragColor = texture(TextureArray, vec3(Texture, 2));
	}
	else if (blockid == 3)
	{
		if (face == 1)
			FragColor = texture(TextureArray, vec3(Texture, blockid));
		else if (face == 0)
			FragColor = texture(TextureArray, vec3(Texture, 6));
		else
			FragColor = texture(TextureArray, vec3(Texture, 4));
	}
	else if (blockid == 5)
	{
		FragColor = vec4(0.4f, 0.4f, 0.9f, 0.2f);
	}
	else
		FragColor = texture(TextureArray, vec3(Texture, blockid));
	if (face == 4 || face == 2)
		FragColor *= 0.6;
	else if (face == 5 || face == 3)
		FragColor *= 0.8;
}
