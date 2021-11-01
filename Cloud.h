#ifndef _CLOUD_H_
#define _CLOUD_H_

#include <string>
#include "SFML/Graphics.hpp"

using namespace std::string_literals;

class Cloud
{
public:
	Cloud(float positionX = 0, float positionY = 0, const std::string& filePath = "graphics/cloud.png"s)
	{
		if (m_textureCloud.loadFromFile(filePath))
		{
			m_spriteCloud.setTexture(m_textureCloud);
			m_spriteCloud.setPosition(positionX, positionY);
		}
		else
		{
			exit(EXIT_FAILURE);
		}
	}

	bool getFlyStatus(void) const
	{
		return m_isCloudFlying;
	}

	void setFlyStatus(bool status)
	{
		m_isCloudFlying = status;
	}

	float getSpeedStatus(void) const
	{
		return m_cloudSpeed;
	}

	void setSpeedStatus(float speed)
	{
		m_cloudSpeed = speed;
	}

	const sf::Texture& getTexture(void) const
	{
		return m_textureCloud;
	}

	sf::Sprite& getSprite(void)
	{
		return m_spriteCloud;
	}

private:
	bool m_isCloudFlying = false;

	float m_cloudSpeed = 0.0f;

	sf::Texture m_textureCloud;
	sf::Sprite m_spriteCloud;
};


#endif	// _BEE_H_