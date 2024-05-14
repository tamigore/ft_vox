#ifndef __BLOCK_HPP__
# define __BLOCK_HPP__

enum BlockType
{
	air,
	grass,
	grassSide,
	snow,
	snowSide,
	water,
	dirt,
	stone,
	bedrock,
	gravel,
	sand,
};

namespace obj
{

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
