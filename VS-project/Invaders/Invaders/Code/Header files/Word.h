#ifndef WORD_H
#define WORD_H

//-----------------------------------------------------------------------------
// Word.h
// The words which the player must type in the second mode of the game.
//-----------------------------------------------------------------------------

// Include our base class.
#include "Renderable.h"

class Word : public IRenderable
{
	public:
		Word(const char* text);
		~Word();

	private:
		// What does the word say?
		const char* m_wordText;
};

#endif // WORD_H