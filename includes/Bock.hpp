#ifndef __BLOCK_HPP__
# define __BLOCK_HPP__

typedef enum BlockType
{
	UNKNOWN,
	DIRT,
	GRASS,
	STONE,
	SAND,
	WATER,
	SNOW,
//	ORES
	IRON_ORE,
	GOLD_ORE,
	DIAMOND_ORE,
//	FLORA
	WOOD,
	LEAVES,
	FLOWER,
	GRASS_TALL,
//	STRUCTURES
};

struct Bock
{
	BlockType	type;
	Mesh		mesh;
	Texture		texture;
	Normals		normals;
};

#endif
