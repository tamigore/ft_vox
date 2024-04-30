#version 330 core 
out	vec4	FragColor;

in	vec2	Texture;
in	float	shadow;
in	float	blockid;
in  float	face;

uniform sampler2DArray TextureArray;

void main()
{
	// int id = int(blockid);
	// if (id == 0)
	if (blockid < 0)
		FragColor = vec4(0.1, 0.1, 0.1, 0.0);
	else if (blockid < 1.5)
		FragColor = texture(TextureArray, vec3(Texture, blockid)) * vec4(0.4, 0.8, 0.6, 0);
	else
		FragColor = texture(TextureArray, vec3(Texture, blockid));
	if (face <= 1)
		FragColor *= 0.5;
	else if (face <= 2)
		FragColor *= 0.5;
}
