#version 330 core 
out vec4 FragColor;

in vec2 Texture;
in vec3 Color;

uniform float mixValue;
uniform bool useColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;

void main()
{
	if (useColor)
		FragColor = vec4(Color, 1.0f);
	else
		FragColor = mix(texture(texture_diffuse1, Texture), texture(texture_diffuse2, Texture), texture(texture_diffuse2, Texture).a * mixValue);

}
