#version 460 core
out vec4 FragColor;

in float	out_light;
in vec2		tex_out;

flat in int out_grass;
flat in int out_type;

uniform int TextureArraySize;

uniform sampler2DArray ourTextureArray;

void main()
{
	uniform int grassColor = vec4(1., 1., 1., 0);
	if (out_grass == 1)
		grassColor = vec4(0.4, 0.9, 0.6, 0);
	FragColor = texture(ourTextureArray, vec3(tex_out, out_type)) * grassColor * out_light;
}
