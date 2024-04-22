#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
// layout (location = 2) in vec3 aColor;

out vec2 Texture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.xzy, 1.0);
	// ourColor = aColor;
	Texture = vec2(aTexCoord.x * 2, aTexCoord.y * 2);
}
