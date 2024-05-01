#include "stb_image.h"
#include "Chunk.hpp"

using namespace obj;

Chunk::Chunk(int x, int y) : posX(x), posY(y)
{
	chunk = new unsigned char[size_x * size_y * size_z];
}

Chunk::~Chunk()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	delete[] chunk;
}

void	Chunk::createFaces( int x, int y, int z, int position, int face, int block)
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
	obj::Vertex tmp;
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
	tmp.TextureCoordonates = texture[0];
	tmp.Position = angle[0];
    tmp.Face = face;
    tmp.Block = block - 1;
	vertices.push_back(tmp);
	tmp.TextureCoordonates = texture[1];
	tmp.Position = angle[1];
	vertices.push_back(tmp);
	tmp.TextureCoordonates = texture[2];
	tmp.Position = angle[2];
	vertices.push_back(tmp);
	tmp.TextureCoordonates = texture[3];
	tmp.Position = angle[3];
	vertices.push_back(tmp);
}

void	Chunk::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoordonates));
    glEnableVertexAttribArray(3);	
    glVertexAttribIPointer(3, 1, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, Face));
    glEnableVertexAttribArray(4);
    glVertexAttribIPointer(4, 1, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, Block));
    glBindVertexArray(0);
	isVAO = true;
}

void	Chunk::draw(Shader &shader)
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void	Chunk::generateFaces(void)
{
	for (int x = 0; x < size_x; x++){
		for (int y = 0; y < size_y; y++){
			for (int z = 0; z < size_z; z++){
				if (!chunk[z + y * size_z + x * size_y * size_z])
					continue;
				int position;
				position = (z - 1) + y * size_z + x * size_y * size_z;
				if (z == 0 || !chunk[position])
					createFaces(posX * size_x + x, posY * size_y + y, z, position, 0, chunk[z + y * size_z + x * size_y * size_z]);
				position = (z + 1) + y * size_z + x * size_y * size_z;
				if (z == 255 || !chunk[position])
					createFaces(posX * size_x + x, posY * size_y + y, z, position, 1, chunk[z + y * size_z + x * size_y * size_z]);
				position = z + (y - 1) * size_z + x * size_y * size_z;
				if (y == 0 || !chunk[position])
					createFaces(posX * size_x + x, posY * size_y + y, z, position, 2, chunk[z + y * size_z + x * size_y * size_z]);
				position = z + (y + 1) * size_z + x * size_y * size_z;
				if (y == 15 || !chunk[position])
					createFaces(posX * size_x + x, posY * size_y + y, z, position, 3, chunk[z + y * size_z + x * size_y * size_z]);
				position = z + y * size_z + (x - 1) * size_y * size_z;
				if (x == 0 || !chunk[position])
					createFaces(posX * size_x + x, posY * size_y + y, z, position, 4, chunk[z + y * size_z + x * size_y * size_z]);
				position = z + y * size_z + (x + 1) * size_y * size_z;
				if (x == 15 || !chunk[position])
					createFaces(posX * size_x + x, posY * size_y + y, z, position, 5, chunk[z + y * size_z + x * size_y * size_z]);
			}
		}
	}
	isCreated	= true;
}

Chunk	*Chunk::getWest() { return west; }
Chunk	*Chunk::getEast() { return east; }
Chunk	*Chunk::getNorth() { return north; }
Chunk	*Chunk::getSouth() { return south; }

void	Chunk::setWest(Chunk *c) { west = c; }
void	Chunk::setEast(Chunk *c) { east = c; }
void	Chunk::setNorth(Chunk *c) { north = c; }
void	Chunk::setSouth(Chunk *c) { south = c; }

// Chunk	*Chunk::getChunk(Chunk *chunk, int x, int y)
// {
// 	while (x != posX && y != posY)
// 	{
// 		if (x < posX)
// 		{
// 			if (west)
// 				;
// 			return nullptr;
// 		}
// 	}
// }
