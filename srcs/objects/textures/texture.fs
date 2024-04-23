#version 330 core 
out	vec4	FragColor;

in	vec2	Texture;
in	float	faceout;
in	float	blockid;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;

void main()
{
	if (blockid < 0.5)
		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	else if (blockid < 1.5)
		FragColor = texture(texture_diffuse1, Texture) * faceout;
	else if (blockid < 2.5)
		FragColor = texture(texture_diffuse2, Texture) * faceout;
	else
		FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
