#ifndef _SOUND_HANDLER_H_
#define _SOUND_HANDLER_H_

#include <string>
#include "SFML/Audio.hpp"

using namespace std::string_literals;

class SoundHandler {
public:
	SoundHandler(const std::string& filePath = "sound/"s)
	{
		m_chopBuffer.loadFromFile(filePath + "chop.wav"s);
		m_chop.setBuffer(m_chopBuffer);

		m_deathBuffer.loadFromFile(filePath + "death.wav"s);
		m_death.setBuffer(m_deathBuffer);

		m_outOfTimeBuffer.loadFromFile(filePath + "out_of_time.wav"s);
		m_oot.setBuffer(m_outOfTimeBuffer);
	}

	sf::Sound& getChop(void)
	{
		return m_chop;
	}

	sf::Sound& getDeath(void)
	{
		return m_death;
	}

	sf::Sound& getNoTime(void)
	{
		return m_oot;
	}

private:
	sf::SoundBuffer m_chopBuffer{};
	sf::Sound m_chop{};

	sf::SoundBuffer m_deathBuffer{};
	sf::Sound m_death{};

	sf::SoundBuffer m_outOfTimeBuffer{};
	sf::Sound m_oot{};
};


#endif // _SOUND_HANDLER_H_