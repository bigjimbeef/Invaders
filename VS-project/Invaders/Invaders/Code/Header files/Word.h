#ifndef WORD_H
#define WORD_H

//-----------------------------------------------------------------------------
// Word.h
// The words which the player must type in the second mode of the game.
//-----------------------------------------------------------------------------

#include <string>

// Include our base class.
#include "Renderable.h"

// Forward declare Game to allow access to Game singleton.
class Game;

class Word : public IRenderable
{
	public:
		Word(IRenderable& owner, const char* text = "");
		~Word();

		// Implement this function to show that this class is not abstract.
		inline void IsAbstract() { /* Do nothing */ }

		virtual void Update(float frameTime);
		virtual void Render();

		// Receives a letter from the player keyboard, passed through the
		// ProjectileManager.
		bool ReceiveLetter(char letter);

		//---------------------------------------------------------------------
		// Accessors
		inline int GetCleared() { return m_lettersCleared; }

	private:
		// Get a random letter.
		char GetRandomLetter();

		// The IRenderable that owns this Word.
		IRenderable& m_owner;

		// What does the word say?
		std::string m_wordText;
		// How many letters have we cleared so far?
		unsigned int m_lettersCleared;

		// The speed of rotation of the Word. Only used for single letters.
		float m_rotation;
		float m_rotationPerFrame;

		static const int MAX_LETTERS = 10;
		// Array of sprites that make up the word.
		IDirect3DTexture9* m_textSprites[MAX_LETTERS];

		// The deviation from base position of each letter.
		float m_verticalDeviation[MAX_LETTERS];
		float m_deviationTime[MAX_LETTERS];

		// Used for spacing and rendering words.
		static const int LETTER_SIZE = 32;
		static const int LETTER_SPACING = 36;

		static const int WIGGLE_FREQUENCY_SCALAR = 100;
		static const int WIGGLE_AMPLITUDE = 3;
};

#endif // WORD_H