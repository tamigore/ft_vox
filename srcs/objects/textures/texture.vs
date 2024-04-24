#version 330 core
layout (location = 0) in vec3	aPos;
layout (location = 1) in vec3	aNormal;
layout (location = 2) in vec2	aTexCoord;
layout (location = 3) in float	aface;
layout (location = 4) in float	ablock;

out	vec2	Texture;
out	float	shadow;
out	float	blockid;
out	float	face;
uniform mat4	model;
uniform mat4	view;
uniform mat4	projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	Texture = vec2(aTexCoord.x , -aTexCoord.y );
	shadow = 1.0f;
	if (aface == 0 || aface == 4)
		shadow = 0.5f;
	blockid = ablock;
	face = aface;
}
