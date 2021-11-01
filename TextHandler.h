#ifndef _TEXT_HANDLER_H_
#define _TEXT_HANDLER_H_

#include <string>
#include "SFML/Graphics.hpp"

class TextHandler
{
public:
	TextHandler(const std::string& filePath = "fonts/KOMIKAP_.ttf")
	{
		// Set the Font for text messages
		font.loadFromFile(filePath);

		m_messageText.setFont(font);
		m_messageText.setString("Press Enter to start!");
		m_messageText.setCharacterSize(75);
		m_messageText.setFillColor(sf::Color::White);

		sf::FloatRect textRect = m_messageText.getLocalBounds();
		m_messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		m_messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

		m_scoreText.setFont(font);
		m_scoreText.setCharacterSize(100);
		m_scoreText.setFillColor(sf::Color::Blue);
		m_scoreText.setPosition(20, 20);		
	}

	sf::Text& getMessageText(void)
	{
		return m_messageText;
	}

	void setMessageText(const std::string& messageText)
	{
		m_messageText.setString(messageText);
	}

	sf::Text& getScoreText(void)
	{
		return m_scoreText;
	}

	void setScoreText(const std::string& scoreText)
	{
		m_scoreText.setString(scoreText);
	}

private:
	sf::Text m_messageText{};
	sf::Text m_scoreText{};

	sf::Font font{};
};


#endif // _TEXT_HANDLER_H_