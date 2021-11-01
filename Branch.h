#ifndef _BRUNCH_H_
#define _BRUNCH_H_

#include <string>
#include "SFML/Graphics.hpp"

using namespace std::string_literals;

enum class branchSide { LEFT, RIGHT, NONE };

class Branch {
public:
	Branch(float positionX = -2000, float positionY = -2000, const std::string& filePath = "graphics/branch.png"s)
	{
		if (m_textureBranch.loadFromFile(filePath))
		{
			m_spriteBranch.setTexture(m_textureBranch);
			m_spriteBranch.setPosition( positionX, positionY);
			m_spriteBranch.setOrigin(220, 20);
		}
		else
		{
			exit(EXIT_FAILURE);
		}
	}

	const sf::Texture& getTexture(void) const
	{
		return m_textureBranch;
	}

	sf::Sprite& getSprite(void)
	{
		return m_spriteBranch;
	}

	branchSide& getBranchPosition(void)
	{
		return m_position;
	}

	void setBranchPosition(const branchSide& branchSide)
	{
		m_position = branchSide;
	}
private:
	branchSide m_position{ branchSide::NONE };

	sf::Texture m_textureBranch;
	sf::Sprite m_spriteBranch;
	
};

#endif // _BRUNCH_H_