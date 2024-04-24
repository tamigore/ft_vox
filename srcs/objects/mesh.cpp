#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include "objects/mesh.hpp"

using namespace obj;

#define OBJL_CONSOLE_OUTPUT

mesh::mesh()
{
	obj::Noise noise(42, 16);
	int size_x = 16;
	int size_y = 16;
	int size_z = 256;
	unsigned char chunk[size_x * size_y * size_z];
	for (int x = 0; x < size_x; x++){
		for (int y = 0; y < size_y; y++){
			float perlin = noise.Generate2D(math::vec2(x *0.047893218043, y * 0.06754896729384), 4, 0.5);
			for (int z = 0; z < size_z; z++){
				if (z < 10 * perlin)
					chunk[z + y * size_z + x * size_y * size_z] = 1;
				else
					chunk[z + y * size_z + x * size_y * size_z] = 0;
			}
		}
	}
	std::cout << "SWITCH" << std::endl;
	for (int x = 0; x < size_x; x++){
		for (int y = 0; y < size_y; y++){
			for (int z = 0; z < size_y; z++){
				if (!chunk[z + y * size_z + x * size_y * size_z])
					continue;
				int position;
				position = (z - 1) + y * size_z + x * size_y * size_z;
				if (z == 0 || !chunk[position])
					createFaces(*this, x, y, z, 0);
				position = (z + 1) + y * size_z + x * size_y * size_z;
				if (z == 255 || !chunk[position])
					createFaces(*this, x, y, z, 1);
				position = z + (y - 1) * size_z + x * size_y * size_z;
				if (y == 0 || !chunk[position])
					createFaces(*this, x, y, z, 2);
				position = z + (y + 1) * size_z + x * size_y * size_z;
				if (y == 15 || !chunk[position])
					createFaces(*this, x, y, z, 3);
				position = z + y * size_z + (x - 1) * size_y * size_z;
				if (x == 0 || !chunk[position])
					createFaces(*this, x, y, z, 4);
				position = z + y * size_z + (x + 1) * size_y * size_z;
				if (x == 15 || !chunk[position])
					createFaces(*this, x, y, z, 5);
			}
		}
	}	
	setupMesh();
}

mesh::mesh(char* path)
{
	if (!loadMesh(path))
	{
		std::cout << "Error: Could not load mesh" << std::endl;
		throw std::exception();
	}
	std::cout << *this << std::endl;
	setupMesh();
}

mesh::~mesh() {}

void mesh::createFaces(obj::mesh &mesh, int x, int y, int z, int face)
{
	math::vec3 angle[4];
	math::vec3 normal;

	switch (face)
	{
		case 0:
			angle[0] = math::vec3(x, y, z);
			angle[1] = math::vec3(x + 1, y, z);
			angle[2] = math::vec3(x + 1, y + 1, z);
			angle[3] = math::vec3(x, y + 1, z);
			normal = math::vec3(0, 0, -1);
			break;
		case 1:
			angle[0] = math::vec3(x, y, z + 1);
			angle[1] = math::vec3(x + 1, y, z + 1);
			angle[2] = math::vec3(x + 1, y + 1, z + 1);
			angle[3] = math::vec3(x, y + 1, z + 1);
			normal = math::vec3(0, 0, 1);
			break;
		case 2:
			angle[0] = math::vec3(x, y, z);
			angle[1] = math::vec3(x + 1, y, z);
			angle[2] = math::vec3(x + 1, y, z + 1);
			angle[3] = math::vec3(x, y, z + 1);
			normal = math::vec3(0, -1, 0);
			break;
		case 3:
			angle[0] = math::vec3(x, y + 1, z);
			angle[1] = math::vec3(x + 1, y + 1, z);
			angle[2] = math::vec3(x + 1, y + 1, z + 1);
			angle[3] = math::vec3(x, y + 1, z + 1);
			normal = math::vec3(0, 1, 0);
			break;
		case 4:
			angle[0] = math::vec3(x, y + 1, z);
			angle[1] = math::vec3(x, y, z);
			angle[2] = math::vec3(x, y, z + 1);
			angle[3] = math::vec3(x, y + 1, z + 1);
			normal = math::vec3(-1, 0, 0);
			break;
		case 5:
			angle[0] = math::vec3(x + 1, y + 1, z);
			angle[1] = math::vec3(x + 1, y, z);
			angle[2] = math::vec3(x + 1, y, z + 1);
			angle[3] = math::vec3(x + 1, y + 1, z + 1);
			normal = math::vec3(1, 0, 0);
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
	mesh.indices.push_back(mesh.vertices.size());	
	mesh.indices.push_back(mesh.vertices.size() + 1);	
	mesh.indices.push_back(mesh.vertices.size() + 2);
	mesh.indices.push_back(mesh.vertices.size());	
	mesh.indices.push_back(mesh.vertices.size() + 2);	
	mesh.indices.push_back(mesh.vertices.size() + 3);
	for (int i = 0; i < 4; i++)
	{
		tmp.Position = angle[i];
		tmp.Texture = texture[i];
		tmp.Normal = normal;
		mesh.vertices.push_back(tmp);
	}
}

void	mesh::draw(shader &shader)
{
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
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void mesh::setupMesh()
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
	std::cout << "Vertex offset Normal : " << offsetof(vertex, Normal) << " vs " << 3 * sizeof(float) << std::endl;
	std::cout << "Vertex offset Texture : " << offsetof(vertex, Texture) << std::endl;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Normal));
	glEnableVertexAttribArray(2);	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Texture));
	glBindVertexArray(0);
}

std::ostream&	obj::operator<<(std::ostream &output, const mesh &input)
{
	output << "Mesh:" << std::endl;
	output << "\rVertex:" << std::endl;
	for (unsigned int i = 0; i < input.vertices.size(); i++)
	{
		output << "Position: " << input.vertices[i].Position << "\t";
		output << "Normal: " << input.vertices[i].Normal << "\t";
		output << "Texture: " << input.vertices[i].Texture << std::endl;
	}
	output << "\rIndices:" << std::endl;
	for (unsigned int i = 0; i < input.indices.size(); i++)
	{
		output << input.indices[i] << " ";
	}
	output << std::endl;
	output << "\rTexture:" << std::endl;
	for (unsigned int i = 0; i < input.textures.size(); i++)
	{
		output << "\r\rid: " << input.textures[i].id << std::endl;	
		output << "\r\rtype: " << input.textures[i].type << std::endl;	
	}
	return output;
}

bool	mesh::add_vertex_position(std::string curline)
{
	std::vector<std::string> tokens;
	split(curline, tokens, " ");
	if (tokens.size() != 3)
	{
		std::cout << "Error: Vertex does not have 3 coordinates" << std::endl;
		return false;
	}
	math::vec3 position;
	position.x = std::stof(tokens[0]);
	position.y = std::stof(tokens[1]);
	position.z = std::stof(tokens[2]);
	if (this->position_indices.size() >= this->vertices.size())
		this->vertices.push_back(vertex());
	this->vertices[this->position_indices.size()].Position = position;
	this->vertices[this->position_indices.size()].Normal = math::vec3(0.0);
	this->vertices[this->position_indices.size()].Texture = math::vec2(0.0);
	this->position_indices.push_back(this->position_indices.size());
	return true;
}

bool	mesh::add_vertex_texture(std::string curline)
{
	std::vector<std::string> tokens;
	split(curline, tokens, " ");
	if (tokens.size() != 2)
	{
		std::cout << "Error: Texture does not have 2 coordinates" << std::endl;
		return false;
	}
	math::vec2 texture;
	texture.x = std::stof(tokens[0]);
	texture.y = std::stof(tokens[1]);
	if (texture.x < 0 || texture.x > 1 || texture.y < 0 || texture.y > 1)
	{
		std::cout << "Error: Texture coordinates are not normalized" << std::endl;
		return false;
	}
	if (this->texture_indices.size() >= this->vertices.size())
	{
		std::cout << "Error: Normal index out of range" << std::endl;
		return false;
	}
	this->vertices[this->texture_indices.size()].Texture = texture;
	this->texture_indices.push_back(this->texture_indices.size());
	return true;
}

bool	mesh::add_vertex_normal(std::string curline)
{
	std::vector<std::string> tokens;
	split(curline, tokens, " ");
	if (tokens.size() != 3)
	{
		std::cout << "Error: Normal does not have 3 coordinates" << std::endl;
		return false;
	}
	math::vec3 normal;
	normal.x = std::stof(tokens[0]);
	normal.y = std::stof(tokens[1]);
	normal.z = std::stof(tokens[2]);
	if (this->normal_indices.size() >= this->vertices.size())
	{
		std::cout << "Error: Normal index out of range" << std::endl;
		return false;
	}
	this->vertices[this->normal_indices.size()].Normal = normal;
	this->normal_indices.push_back(this->normal_indices.size());	
	return true;
}

bool	mesh::add_face(std::string pram)
{
	std::vector<unsigned int> vertices;
	std::vector<unsigned int> normals;
	std::vector<unsigned int> textures;
	std::vector<std::string> tokens;
	split(pram, tokens, " ");
	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		std::vector<std::string> face_tokens;
		if (tokens[i].find("/") != std::string::npos)
		{
			split(tokens[i], face_tokens, "/");
			vertices.push_back(std::stoul(face_tokens[0]));
			normals.push_back(std::stoul(face_tokens[1]));
			if (!face_tokens[2].empty())
				textures.push_back(std::stoul(face_tokens[2]));
			else
				textures.push_back(0);
		}
		else if (tokens[i].find("//") != std::string::npos)
		{
			vertices.push_back(std::stoul(face_tokens[0]));
			normals.push_back(0);
			textures.push_back(std::stoul(face_tokens[1]));
		}
		else
		{
			vertices.push_back(std::stoul(tokens[i]));
			normals.push_back(0);
			textures.push_back(0);
		}
	}
	for (unsigned int i = 0; i < vertices.size() - 2; i++)
	{
		this->indices.push_back(vertices[0] - 1);
		this->indices.push_back(vertices[i + 1] - 1);
		this->indices.push_back(vertices[i + 2] - 1);
	}
	return true;
}

bool	mesh::add_texture(const char *name, const char *path)
{
	texture new_texture;
	new_texture.id = TextureFromFile(name, path);
	new_texture.type = "texture_diffuse";
	new_texture.path = std::string(path) + "/" + std::string(path);
	this->textures.push_back(new_texture);
	return true;
}

unsigned int	mesh::TextureFromFile(const char *path, const std::string &directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	std::cout << filename << std::endl;
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
		std::cout << "Format: " << (format == GL_RGBA ? "RGBA" : "RGB") << std::endl;

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

bool	mesh::loadMesh(const char* path)
{
	std::string str_path = path;
	if (str_path.empty() || str_path.size() < 4 || str_path.substr(str_path.size() - 4, 4) != ".obj")
		return false;
	std::ifstream file(str_path);
	if (!file.is_open()) {
		std::cout << "Error: Could not open file " << str_path << std::endl;
		return false;
	}

	std::string curline;
	while (std::getline(file, curline))
	{
		if (curline.empty())
			continue;
		std::string prefix = firstToken(curline);
		std::string param = tail(curline);
		#ifdef OBJL_CONSOLE_OUTPUT
		std::cout << "\r- " << prefix << " " << param << std::endl;
		#endif
		if (prefix == "o")
		{
			this->name = param;
		}
		else if (prefix == "v")
		{
			if (!add_vertex_position(param))
				return false;
		}
		else if (prefix == "vt")
		{
			if (!add_vertex_texture(param))
				return false;
		}
		else if (prefix == "vn")
		{
			if (!add_vertex_normal(param))
				return false;
		}
		else if (prefix == "f")
		{
			if (!add_face(param))
				return false;
		}
	}
	center_around_orgin();
	facesDuplicateVertexes();
	setupMesh();
	std::cout << "Mesh Loaded" << std::endl;
	return true;
}

void	mesh::min_max_bounds(math::vec3& min_bound, math::vec3& max_bound)
{
	for (std::vector<obj::vertex>::iterator vert = this->vertices.begin(); vert != this->vertices.end();) 
	{
		min_bound[0] = std::min(min_bound[0], vert->Position[0]);
		min_bound[1] = std::min(min_bound[1], vert->Position[1]);
		min_bound[2] = std::min(min_bound[2], vert->Position[2]);

		max_bound[0] = std::max(max_bound[0], vert->Position[0]);
		max_bound[1] = std::max(max_bound[1], vert->Position[1]);
		max_bound[2] = std::max(max_bound[2], vert->Position[2]);
		vert++;
	}
}

void	mesh::center_around_orgin()
{
	math::vec3 min_bound = {0, 0, 0};
	math::vec3 max_bound = {0, 0, 0};

	min_max_bounds(min_bound, max_bound);

	math::vec3 center = {0, 0, 0};
	
	center[0] = (min_bound[0] + max_bound[0]) / 2;
	center[1] = (min_bound[1] + max_bound[1]) / 2;
	center[2] = (min_bound[2] + max_bound[2]) / 2;

	for (std::vector<obj::vertex>::iterator vert = this->vertices.begin(); vert != this->vertices.end(); ++vert)  {
		(vert->Position)[0] -= center[0];
		(vert->Position)[1] -= center[1];
		(vert->Position)[2] -= center[2];
	}
}

void	mesh::facesDuplicateVertexes()
{
	std::map<unsigned int, unsigned int> nbPosIndex;
	std::vector<math::vec2> textures = {
		{0.0, 0.0},
		{0.0, 1.0},
		{1.0, 0.0},
		{1.0, 1.0}
	};
	int face = 0;
	for (std::vector<unsigned int>::iterator it = this->indices.begin(); it != this->indices.end(); ++it)
	{
		if (nbPosIndex.find(*it) == nbPosIndex.end())
		{
			nbPosIndex[*it] = 1;
			this->vertices[*it].Texture = textures[face % 4];
		}
		else
		{
			this->vertices.push_back(this->vertices[*it]);
			*it = this->vertices.size() - 1;
			nbPosIndex[*it] += 1;
			this->vertices[*it].Texture = textures[face % 4];
		}
		face++;
	}
}