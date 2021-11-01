#ifndef _GRAVE_STONE_H_
#define _GRAVE_STONE_H_

#include <string>
#include "SFML/Graphics.hpp"

using namespace std::string_literals;

class Gravestone
{
public:
	Gravestone(float positionX = 0, float positionY = 0, const std::string& filePath = "graphics/rip.png"s)
	{
		if (m_textureRIP.loadFromFile(filePath))
		{
			m_spriteRIP.setTexture(m_textureRIP);
			m_spriteRIP.setPosition(positionX, positionY);
		}
		else
		{
			exit(EXIT_FAILURE);
		}
	}

	const sf::Texture& getTexture(void) const
	{
		return m_textureRIP;
	}

	sf::Sprite& getSprite(void)
	{
		return m_spriteRIP;
	}

private:
	sf::Texture m_textureRIP;
	sf::Sprite m_spriteRIP;
};

#endif // _GRAVE_STONE_H_