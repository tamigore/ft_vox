#include "objects/Skybox.hpp"
#include "stb_image.h"

using namespace obj;


Skybox::Skybox()
{
	skyboxShader.load("srcs/skybox/skybox.vs", "srcs/skybox/skybox.fs", nullptr);
}

Skybox::~Skybox()
{
}

void	Skybox::draw(math::mat4 view, math::mat4 projection)
{
	// draw skybox as last
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);

	skyboxShader.use();
	view[3][0] = 0;
	view[3][1] = 0;// a.x a.y a.z 0
	view[3][2] = 0;// b.x b.y b.z 0
	view[3][3] = 0;// c.x c.y c.z 0
	view[0][3] = 0;// 0   0   0   0
	view[1][3] = 0;
	view[2][3] = 0;
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);

	skyboxShader.use();
	skyboxShader.setMat4("projection", projection);
	skyboxShader.setMat4("view", view);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}

void	Skybox::setup()
{
		// COORDINATES
	float skyboxVertices[] = {
		-1.0f,	-1.0f,	 1.0f,//        7--------6
		 1.0f,	-1.0f,	 1.0f,//       /|       /|
		 1.0f,	-1.0f,	-1.0f,//      4--------5 |
		-1.0f,	-1.0f,	-1.0f,//      | |      | |
		-1.0f,	 1.0f,	 1.0f,//      | 3------|-2
		 1.0f,	 1.0f,	 1.0f,//      |/       |/
		 1.0f,	 1.0f,	-1.0f,//      0--------1
		-1.0f,   1.0f,	-1.0f
	};

	// INDICES
	unsigned int skyboxIndices[] =
	{
		// Right
		1, 2, 6,
		6, 5, 1,
		// Left
		0, 4, 7,
		7, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4,
		// Bottom
		0, 3, 2,
		2, 1, 0,
		// Back
		0, 1, 5,
		5, 4, 0,
		// Front
		3, 7, 6,
		6, 2, 3
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void	Skybox::load()
{
	// SKY BOX IMAGES
	std::vector<const char *> files = {
		"srcs/skybox/sky/right.jpg",
		"srcs/skybox/sky/left.jpg",
		"srcs/skybox/sky/top.jpg",
		"srcs/skybox/sky/bottom.jpg",
		"srcs/skybox/sky/front.jpg",
		"srcs/skybox/sky/back.jpg"
	};

	for (unsigned int i = 0; i < files.size(); i++)
	{
		std::cout << files[i] << std::endl;

		int width, height, nrChannels;
		unsigned char* data = stbi_load(files[i], &width, &height, &nrChannels, 0);
		GLenum format = channelColor(nrChannels);
		if (data)
		{
			// stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				format,
				width,
				height,
				0,
				format,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << files[i] << std::endl;
			stbi_image_free(data);
		}
	}
}
