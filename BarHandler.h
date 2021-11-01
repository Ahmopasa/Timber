#ifndef _BAR_HANDLER_H_
#define _BAR_HANDLER_H_

#include "SFML/Graphics.hpp"
#include <tuple>


class BarHandler {
public:
	BarHandler(void)
	{
		m_timeBar.setSize(sf::Vector2f{ m_barWidth, m_barHeight });
		m_timeBar.setFillColor(sf::Color::Red);
		m_timeBar.setPosition(static_cast<float>(1920/2.0f - m_barWidth / 2.0f), 980);
	}

	float getRemainingGameTime(void) const
	{
		return m_remainingGameTime;
	}

	void setRemainingGameTime(float gameTime)
	{
		m_remainingGameTime = gameTime;
	}

	sf::RectangleShape& getTimeBar(void)
	{
		return m_timeBar;
	}

	void setTimeBar(const sf::RectangleShape& newTimeBar)
	{
		m_timeBar = newTimeBar;
	}

	std::tuple<float, float, float> getTimeBarVariables(void)
	{
		return { m_shrinkRatio, m_remainingGameTime, m_barHeight };
	}
private:
	float m_remainingGameTime{ 6.0f };
	float m_barWidth{400};
	float m_shrinkRatio{ m_barWidth / m_remainingGameTime };
	float m_barHeight{ 80 };

	sf::RectangleShape m_timeBar{};
	sf::Time m_totalGameTime{};
};

#endif // _BAR_HANDLER_H_