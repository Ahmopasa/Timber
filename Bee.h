#ifndef _BEE_H_
#define _BEE_H_

#include <string>
#include "SFML/Graphics.hpp"

using namespace std::string_literals;

class Bee
{
public:
	Bee(float positionX = 0, float positionY = 0, const std::string& filePath = "graphics/bee.png"s)
	{
		if (m_textureBee.loadFromFile(filePath))
		{
			m_spriteBee.setTexture(m_textureBee);
			m_spriteBee.setPosition(positionX, positionY);
		}
		else
		{
			exit(EXIT_FAILURE);
		}
	}

	bool getFlyStatus(void) const
	{
		return m_isBeeFlying;
	}

	void setFlyStatus(bool status)
	{
		m_isBeeFlying = status;
	}

	float getSpeedStatus(void) const
	{
		return m_beeSpeed;
	}

	void setSpeedStatus(float speed)
	{
		m_beeSpeed = speed;
	}

	const sf::Texture& getTexture(void) const
	{
		return m_textureBee;
	}

	sf::Sprite& getSprite(void)
	{
		return m_spriteBee;
	}

private:
	bool m_isBeeFlying{ false };

	float m_beeSpeed{ 0.0f };

	sf::Texture m_textureBee;
	sf::Sprite m_spriteBee;
};

#endif	// _BEE_H_