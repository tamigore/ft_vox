#ifndef __BLOCK_HPP__
# define __BLOCK_HPP__

namespace obj
{

int BLOCK_RENDER_SIZE = 1;

enum BlockType
{
	air,
	dirt,
	grass,
	stone,
	bedrock,
	sand,
	oak_wood_side,
	leaves_2,
	water,
	snow,
	iron_ore,
	gold_ore ,
	diamond_ore,
	UNKNOWN
};

class Block
{
	public:
		Block();
		~Block();

		bool		IsActive() { return m_active; }
		void		SetActive(bool active) { m_active = active; }
		BlockType	GetType() { return m_blockType; }
		void		SetType(BlockType type) { m_blockType = type; }

	private:
		bool		m_active;
		BlockType	m_blockType;
};

}


#endif
