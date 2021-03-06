#pragma once
#include "GameObject.h"
#include "SFML/Audio.hpp"
#include "dllclasses.h"


/**
* \brief CLASS NOT IMPLEMENTED IN THIS FRAMEWORK VERSION.
*  Will generate a stand alone object that contain a playable music track.
*/
class DECLSPEC MusicObject : public GameObject
{
	friend class Application;
private:
	sf::Music* music = nullptr;
	bool looping = false;
	float volume = 0;

protected:
	bool playOnStart = false;

public:
	MusicObject(std::string musicPath, bool isLooping, float volume, bool playOnStart)
	{
		music = new sf::Music();
		music->openFromFile(musicPath);
		music->setLoop(isLooping);
		music->setVolume(volume);
		this->playOnStart = playOnStart;
	}


	~MusicObject() { delete music; }

	sf::SoundSource::Status GetStatus() { return music->getStatus(); }

	void Stop() { music->stop(); }
	void Play() { music->play(); }

};