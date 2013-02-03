#include "AudioManager.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

AudioManager::AudioManager() :
	m_musicPath("Code/Resource files/Audio/funnybg.wav"),
	mp_musicStream(NULL),
	m_musicVolume(1.0f),
	m_soundVolume(1.0f),
	m_musicChannel(0),
	m_musicFrequency(FORTY_FOUR_K),
	m_playbackSpeed(1.0f),
	m_lastPlaybackSpeed(1.0f),
	m_sounds()
{
	// Initialise FMOD.
	FSOUND_Init(44100, 42, 0);
}
AudioManager::~AudioManager()
{
	// TODO: Free all sound effects.
	StopMusic();
}

void AudioManager::Update(float frameTime)
{
	// Check to see if the playback speed has been altered.
	if ( m_playbackSpeed != m_lastPlaybackSpeed )
	{
		// Calculate the new frequency to play the music at.
		int newFreq = static_cast<int>(m_musicFrequency * m_playbackSpeed);

		// Now set the frequency.
		FSOUND_SetFrequency(m_musicChannel, newFreq);

		// TODO: Set all frequency-modulated sound effect frequencies.

		m_lastPlaybackSpeed = m_playbackSpeed;
	}
}

void AudioManager::PlayMusic(bool stopCurrent)
{
	if ( stopCurrent && mp_musicStream )
	{
		StopMusic();
	}
	else if ( mp_musicStream )
	{
		return;
	}

	mp_musicStream = FSOUND_Stream_Open(m_musicPath.c_str(), FSOUND_LOOP_NORMAL, 0, 0);
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

void AudioManager::LoadSoundEffect(std::string effectPath, bool looping)
{
	int flags = 0;
	if ( looping ) 
	{
		flags |= FSOUND_LOOP_NORMAL;
	}

	// Load the sound, using the path provided.
	FSOUND_SAMPLE* soundEffect = 
		FSOUND_Sample_Load(FSOUND_FREE, effectPath.c_str(), flags, 0, 0);

	// Now add the fully loaded sound to the sound effect map. 
	m_sounds[effectPath] = soundEffect;
}

int AudioManager::PlaySoundEffect(std::string name)
{
	// Locate the sound in the sound effect map using its name.
	FSOUND_SAMPLE* soundEffect = NULL;
	
	std::map<std::string, FSOUND_SAMPLE*>::const_iterator cIt;
	cIt = m_sounds.find(name);
	if ( cIt != m_sounds.end() )
	{
		soundEffect = cIt->second;
	}

	if ( !soundEffect )
	{
		return -1;
	}

	int channel = FSOUND_PlaySound(FSOUND_FREE, soundEffect);

	int volume = static_cast<int>(m_soundVolume * 255);
	FSOUND_SetVolume(channel, volume);

	return channel;
}

void AudioManager::StopSoundEffect(int channel)
{
	if ( channel <= 0 )
	{
		return;
	}
	FSOUND_StopSound(channel);
}

void AudioManager::SetPlaybackSpeed(float value)
{
	m_playbackSpeed = value; 
	MathsHelper::Clamp(m_playbackSpeed, 0.0f, 1.0f);
}
