#ifndef _BACK_GROUND_H_H
#define _BACK_GROUND_H_H

#include <string>
#include "SFML/Graphics.hpp"

using namespace std::string_literals;

class BackgroundHandler
{
public:
	BackgroundHandler(const std::string& filePath = "graphics/background.png"s)
	{
		if (m_textureBackground.loadFromFile(filePath))
		{
			m_spriteBackground.setTexture(m_textureBackground);
			m_spriteBackground.setPosition(0, 0);
		}
		else
		{
			exit(EXIT_FAILURE);
		}
	}

	const sf::Texture& getTexture(void) const
	{
		return m_textureBackground;
	}

	sf::Sprite& getSprite(void)
	{
		return m_spriteBackground;
	}
private:
	sf::Texture m_textureBackground;
	sf::Sprite m_spriteBackground;
};
#endif // _BACK_GROUND_H_H