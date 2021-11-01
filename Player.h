#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>
#include "SFML/Graphics.hpp"

using namespace std::string_literals;

enum class playerSide { LEFT, RIGHT, NONE };

class Player
{
public:
	Player(float positionX = 0, float positionY = 0, const std::string& filePath = "graphics/player.png"s)
	{
		if (m_texturePlayer.loadFromFile(filePath))
		{
			m_spritePlayer.setTexture(m_texturePlayer);
			m_spritePlayer.setPosition(positionX, positionY);
		}
		else
		{
			exit(EXIT_FAILURE);
		}
	}

	const sf::Texture& getTexture(void) const
	{
		return m_texturePlayer;
	}

	sf::Sprite& getSprite(void)
	{
		return m_spritePlayer;
	}

	playerSide& getBranchPosition(void)
	{
		return m_position;
	}

	void setBranchPosition(const playerSide& branchSide)
	{
		m_position = branchSide;
	}
private:
	playerSide m_position{ playerSide::LEFT };

	sf::Texture m_texturePlayer;
	sf::Sprite m_spritePlayer;
	
};

#endif // _PLAYER_H_