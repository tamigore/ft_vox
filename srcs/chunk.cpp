# include "../includes/stb_image.h"
#include "../includes/chunk.hpp"

using namespace obj;

Chunk::	Chunk(int x, int y) : posX(x), posY(y)
{
	obj::Noise noise(42, 16);
	obj::Noise noise2(3131, 16);
	int size_x = 16;
	int size_y = 16;
	int size_z = 256;
	unsigned char chunk[size_x * size_y * size_z];
	for (int x = 0; x < size_x; x++){
		for (int y = 0; y < size_y; y++){
			float px, py;
			px = posX * size_x + x;
			py = posY * size_y + y;
			int perlin = 2 + 10 * noise.Generate2D(math::vec2(px * 0.0047893218043 + 10000, py *0.003123123123123 + 10000), 4, 0.5);
			int perlinoise = 30 * noise2.Generate2D(math::vec2(px * 0.0047893218043 + 10000, py *0.003123123123123 + 10000), 4, 0.5);
            for (int z = 0; z < size_z; z++)
			{
				if (z < perlin)
					chunk[z + y * size_z + x * size_y * size_z] = 2;
				else if (z < perlinoise)
					chunk[z + y * size_z + x * size_y * size_z] = 1;
				else
					chunk[z + y * size_z + x * size_y * size_z] = 0;
			}
		}
	}
	for (int x = 0; x < size_x; x++){
		for (int y = 0; y < size_y; y++){
			for (int z = 0; z < size_z; z++){
				if (!chunk[z + y * size_z + x * size_y * size_z])
					continue;
				int position;
				position = (z - 1) + y * size_z + x * size_y * size_z;
				if (z == 0 || !chunk[position])
					createFaces(posX * size_x + x, posY * size_y + y, z, position, 0.0, chunk[z + y * size_z + x * size_y * size_z]);
				position = (z + 1) + y * size_z + x * size_y * size_z;
				if (z == 255 || !chunk[position])
					createFaces(posX * size_x + x, posY * size_y + y, z, position, 1.0, chunk[z + y * size_z + x * size_y * size_z]);
				position = z + (y - 1) * size_z + x * size_y * size_z;
				if (y == 0 || !chunk[position])
					createFaces(posX * size_x + x, posY * size_y + y, z, position, 2.0, chunk[z + y * size_z + x * size_y * size_z]);
				position = z + (y + 1) * size_z + x * size_y * size_z;
				if (y == 15 || !chunk[position])
					createFaces(posX * size_x + x, posY * size_y + y, z, position, 3.0, chunk[z + y * size_z + x * size_y * size_z]);
				position = z + y * size_z + (x - 1) * size_y * size_z;
				if (x == 0 || !chunk[position])
					createFaces(posX * size_x + x, posY * size_y + y, z, position, 4.0, chunk[z + y * size_z + x * size_y * size_z]);
				position = z + y * size_z + (x + 1) * size_y * size_z;
				if (x == 15 || !chunk[position])
					createFaces(posX * size_x + x, posY * size_y + y, z, position, 5.0, chunk[z + y * size_z + x * size_y * size_z]);
			}
		}
	}
}

Chunk::~Chunk()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Chunk::createFaces( int x, int y, int z, int position, int face, int block)
{
	math::vec3 angle[4];
	(void)position;

	switch (face)
	{
	case 0:
		angle[0] = math::vec3(x, y, z);
		angle[1] = math::vec3(x + 1, y, z);
		angle[2] = math::vec3(x + 1, y + 1, z);
		angle[3] = math::vec3(x, y + 1, z);
		break;
	case 1:
		angle[0] = math::vec3(x, y, z + 1);
		angle[1] = math::vec3(x + 1, y, z + 1);
		angle[2] = math::vec3(x + 1, y + 1, z + 1);
		angle[3] = math::vec3(x, y + 1, z + 1);
		break;
	case 2:
		angle[0] = math::vec3(x, y, z);
		angle[1] = math::vec3(x + 1, y, z);
		angle[2] = math::vec3(x + 1, y, z + 1);
		angle[3] = math::vec3(x, y, z + 1);
		break;
	case 3:
		angle[0] = math::vec3(x, y + 1, z);
		angle[1] = math::vec3(x + 1, y + 1, z);
		angle[2] = math::vec3(x + 1, y + 1, z + 1);
		angle[3] = math::vec3(x, y + 1, z + 1);
		break;
	case 4:
		angle[0] = math::vec3(x, y + 1, z);
		angle[1] = math::vec3(x, y, z);
		angle[2] = math::vec3(x, y, z + 1);
		angle[3] = math::vec3(x, y + 1, z + 1);
		break;
	case 5:
		angle[0] = math::vec3(x + 1, y + 1, z);
		angle[1] = math::vec3(x + 1, y, z);
		angle[2] = math::vec3(x + 1, y, z + 1);
		angle[3] = math::vec3(x + 1, y + 1, z + 1);
		break;
	default:
		break;
	}
	obj::vertex tmp;
	std::vector<math::vec2> texture = {
		math::vec2(0.0f, 0.0f),
		math::vec2(1.0f, 0.0f),
		math::vec2(1.0f, 1.0f),
		math::vec2(0.0f, 1.0f)
	};

	indices.push_back(vertices.size());	
	indices.push_back(vertices.size() + 1);	
	indices.push_back(vertices.size() + 2);
	indices.push_back(vertices.size());	
	indices.push_back(vertices.size() + 2);	
	indices.push_back(vertices.size() + 3);
	tmp.Texture = texture[0];
	tmp.Position = angle[0];
    tmp.face = face;
    tmp.block = block;
	vertices.push_back(tmp);
	tmp.Texture = texture[1];
	tmp.Position = angle[1];
	vertices.push_back(tmp);
	tmp.Texture = texture[2];
	tmp.Position = angle[2];
	vertices.push_back(tmp);
	tmp.Texture = texture[3];
	tmp.Position = angle[3];
	vertices.push_back(tmp);
}

void Chunk::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Position));
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Normal));
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Texture));
    glEnableVertexAttribArray(3);	
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, face));
    glEnableVertexAttribArray(4);	
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, block));
    // vertex tangent
    // glEnableVertexAttribArray(3);
    // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Tangent));
    // // vertex bitangent
    // glEnableVertexAttribArray(4);
    // glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Bitangent));
    // // ids
    // glEnableVertexAttribArray(5);
    // glVertexAttribIPointer(5, 4, GL_INT, sizeof(vertex), (void*)offsetof(vertex, m_BoneIDs));

    // // weights
    // glEnableVertexAttribArray(6);
    // glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, m_Weights));
    glBindVertexArray(0);
	isVAO = true;
}

void Chunk::draw(shader &shader)
{
    // draw chunk
	unsigned int diffuseNr  = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr   = 1;
	unsigned int heightNr   = 1;
	for(unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string name = textures[i].type;
		std::string number;
		if(name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if(name == "texture_specular")
			number = std::to_string(specularNr++);
		else if(name == "texture_normal")
			number = std::to_string(normalNr++);
		else if(name == "texture_height")
			number = std::to_string(heightNr++);

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}


bool	Chunk::add_texture(const char *name, const char *path)
{
	texture new_texture;
	new_texture.id = TextureFromFile(name, path);
	new_texture.type = "texture_diffuse";
	new_texture.path = std::string(path) + "/" + std::string(path);
	this->textures.push_back(new_texture);
	return true;
}

unsigned int	Chunk::TextureFromFile(const char *path, const std::string &directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
