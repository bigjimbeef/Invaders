#include "AudioManager.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

AudioManager::AudioManager() :
	m_musicPath("Audio/bgmusic.mp3"),
	mp_musicStream(NULL),
	m_musicVolume(1.0f),
	m_soundVolume(1.0f),
	m_musicChannel(0),
	m_musicFrequency(FORTY_FOUR_K),
	m_playbackSpeed(1.0f),
	m_lastPlaybackSpeed(1.0f)
{

}
AudioManager::~AudioManager()
{
	// TODO: Free all sounds.
}

void AudioManager::Update(float frameTime)
{
	// Check to see if the playback speed has been altered.
	if ( m_playbackSpeed != m_lastPlaybackSpeed )
	{
		// Calculate the new frequency to play the music at.
		float newFreq = m_musicFrequency * m_playbackSpeed;

		// Now set the frequency.
		// FSOUND_SetFrequency(m_musicChannel, newFreq);

		m_lastPlaybackSpeed = m_playbackSpeed;
	}
}

void AudioManager::PlayMusic()
{
	if (mp_musicStream)
	{
		StopMusic();	
	}

	mp_musicStream = FSOUND_Stream_Open(m_musicPath, FSOUND_LOOP_NORMAL, 0, 0);
	m_musicChannel = FSOUND_Stream_Play(FSOUND_FREE, mp_musicStream);

	int volume = static_cast<int>(m_musicVolume * 255);
	FSOUND_SetVolume(m_musicChannel, volume);

	m_musicFrequency = FSOUND_GetFrequency(m_musicChannel);
}

void AudioManager::StopMusic()
{
	if ( mp_musicStream )
	{
		FSOUND_Stream_Close(mp_musicStream);
		mp_musicStream = NULL;
	}
}


void AudioManager::SetPlaybackSpeed(float value)
{
	m_playbackSpeed = value; 
	MathsHelper::Clamp(m_playbackSpeed, 0.0f, 1.0f);
}