#version 460 core
out	vec4	FragColor;

in vec2		Texture;
in float	shadow;
flat in int	blockid;
flat in int	face;

uniform sampler2DArray TextureArray;

void main()
{
	if (blockid == 0)
		FragColor = vec4(0.0, 0.0 , 0.0, 0.0);
	else if (blockid == 1)
		FragColor = texture(TextureArray, vec3(Texture, blockid)) * vec4(0.4, 0.8, 0.6, 0);
	else
		FragColor = texture(TextureArray, vec3(Texture, blockid));
	if (face == 4 || face == 2)
		FragColor *= 0.6;
	else if (face == 5 || face == 3)
		FragColor *= 0.8;
}
