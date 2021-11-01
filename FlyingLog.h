#ifndef _FLYING_LOG_H_
#define _FLYING_LOG_H_

#include <string>
#include "SFML/Graphics.hpp"

using namespace std::string_literals;

class FlyingLog {
public:
	FlyingLog(float positionX = 0, float positionY = 0, const std::string& filePath = "graphics/log.png"s)
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

	const float getPositionLeft(void)const
	{
		return m_POSITON_LEFT;
	}

	const float getPositionRight(void)const
	{
		return m_POSITON_RIGHT;
	}

	float getSpeedX(void)const
	{
		return m_speedX;
	}

	void setSpeedX(float speed)
	{
		m_speedX = speed;
	}

	float getSpeedY(void)const
	{
		return m_speedY;
	}

	void setSpeedY(float speed)
	{
		m_speedY = speed;
	}

	bool getCutStatus(void) const
	{
		return m_isPieceCut;
	}

	void setCutStatus(bool status)
	{
		m_isPieceCut = status;
	}

private:
	bool m_isPieceCut = false;

	float m_speedX{ 1000.0f };
	float m_speedY{ -1500.0f };

	const float m_POSITON_LEFT{ 700.0f };
	const float m_POSITON_RIGHT{ 1075.0f };

	sf::Texture m_textureAxe;
	sf::Sprite m_spriteAxe;
};


#endif // _FLYING_LOG_H_
