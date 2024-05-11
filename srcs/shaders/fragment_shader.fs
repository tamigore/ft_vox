#version 330 core
out	vec4	FragColor;

in vec2		Texture;
in float	shadow;
flat in int	blockid;
flat in int	face;

uniform sampler2DArray TextureArray;

void main()
{
	if (blockid == 1)
	{
		if (face == 1)
			FragColor = texture(TextureArray, vec3(Texture, blockid));
		else if (face == 0)
			FragColor = texture(TextureArray, vec3(Texture, blockid + 2));
		else
			FragColor = texture(TextureArray, vec3(Texture, blockid + 1));
	}
	else
		FragColor = texture(TextureArray, vec3(Texture, blockid));
	if (face == 4 || face == 2)
		FragColor *= 0.6;
	else if (face == 5 || face == 3)
		FragColor *= 0.8;
}
