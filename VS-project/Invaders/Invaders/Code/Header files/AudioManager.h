#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

//-----------------------------------------------------------------------------
// AudioManager.h
// Manages loading/unloading of audio files, and also handles all sounds within
// the game.
//-----------------------------------------------------------------------------

#include <string>
#include <map>

// Forward declare Game to allow access to Game singleton.
class Game;
struct FSOUND_STREAM;
struct FSOUND_SAMPLE;

class AudioManager
{
	public:
		AudioManager();
		~AudioManager();

		void Update(float frameTime);

		// Start playing the music.
		void PlayMusic(bool stopCurrent = false);
		void StopMusic();

		void LoadSoundEffect(std::string name, std::string effectPath, 
							 bool looping = false);
		// Returns the channel the sound is playing in.
		int PlaySoundEffect(std::string name, int volume = -1);
		void StopSoundEffect(int channel);

		//---------------------------------------------------------------------
		// Accessors.
		inline int GetMusicChannel() const { return m_musicChannel; }
		inline int GetMusicFrequency() const { return m_musicFrequency; }
		inline float GetPlaybackSpeed() const { return m_playbackSpeed; }
		void SetPlaybackSpeed(float value);

	private:
		// The file path to the music track.
		std::string m_musicPath;

		// The FMOD stream object that contains the music.
		FSOUND_STREAM* mp_musicStream;

		float m_musicVolume;
		float m_soundVolume;

		// This will hold the channel that that sound is playing in.
		int m_musicChannel;
		// The frequency that the music is playing at, determined at run-time.
		int m_musicFrequency;
		static const int FORTY_FOUR_K = 44100;
		// This is the externally-visible notion of music speed.
		float m_playbackSpeed;
		// Used for debouncing playback speed alteration requests.
		float m_lastPlaybackSpeed;

		// This container is a map of file path to FSOUND_SAMPLE objects.
		std::map<std::string, FSOUND_SAMPLE*> m_sounds;

};

#endif // AUDIOMANAGER_H