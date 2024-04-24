#version 330 core 
out	vec4	FragColor;

in	vec2	Texture;
in	float	shadow;
in	float	blockid;
in  float face;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;

void main()
{
	if (blockid < 0.5)
		FragColor = vec4(0.0, 1.0, 0.0, 1.0);
	else if (blockid < 1.5 && face == 1)
		FragColor = texture(texture_diffuse3, Texture)* shadow;
	else
		FragColor = texture(texture_diffuse2, Texture)* shadow ;
	// else if (blockid < 2.5)
	// 	FragColor = texture(texture_diffuse3, Texture) * shadow;
}
