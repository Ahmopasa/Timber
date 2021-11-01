#ifndef _AXE_H_
#define _AXE_H_

#include <string>
#include "SFML/Graphics.hpp"

using namespace std::string_literals;

class Axe {
public:
	Axe(float positionX = 0, float positionY = 0, const std::string& filePath = "graphics/axe.png"s)
	{
		if (m_textureAxe.loadFromFile(filePath))
		{
			m_spriteAxe.setTexture(m_textureAxe);
			m_spriteAxe.setPosition(positionX, positionY);
		}
		else
		{
			exit(EXIT_FAILURE);
		}
	}

	const sf::Texture& getTexture(void) const
	{
		return m_textureAxe;
	}

	sf::Sprite& getSprite(void)
	{
		return m_spriteAxe;
	}

	std::tuple<float, float> getAxePositions(void)
	{
		return { m_POSITON_LEFT, m_POSITON_RIGHT };
	}
private:
	const float m_POSITON_LEFT{ 700.0f };
	const float m_POSITON_RIGHT{ 1075.0f };

	sf::Texture m_textureAxe;
	sf::Sprite m_spriteAxe;
};


#endif
