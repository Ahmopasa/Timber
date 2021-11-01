#ifndef _TREE_H_
#define _TREE_H_

#include <string>
#include "SFML/Graphics.hpp"

using namespace std::string_literals;

class Tree
{
public:
	Tree(float positionX = 0, float positionY = 0, const std::string& filePath = "graphics/tree.png"s)
	{
		if (m_textureTree.loadFromFile(filePath))
		{
			m_spriteTree.setTexture(m_textureTree);
			m_spriteTree.setPosition(positionX, positionY);
		}
		else
		{
			exit(EXIT_FAILURE);
		}
	}

	const sf::Texture& getTexture(void) const
	{
		return m_textureTree;
	}

	sf::Sprite& getSprite(void)
	{
		return m_spriteTree;
	}
private:
	sf::Texture m_textureTree;
	sf::Sprite m_spriteTree;
};


#endif	// _TREE_H_