#ifndef DICTIONARY_H
#define DICTIONARY_H

//-----------------------------------------------------------------------------
// Dictionary.h
// Manages the list of words that are read from the dictionary file.
//-----------------------------------------------------------------------------

#include <string>
#include <map>
#include <fstream>

// Forward declare Game to allow access to Game singleton.
class Game;

class Dictionary
{
	public:
		Dictionary();
		~Dictionary();

		// Returns a random word from the word list.
		std::string GetRandomWord();

	private:
		// Open the dictionary file and process information into the map.
		void ReadDictionary();

		// The file containing the words.
		std::string m_filePath;
		std::ifstream m_inputFile;

		// A map of the words read from the dictionary file, indexed by length.
		std::multimap<int, std::string> m_wordMap;

		static const int BASE_WORD_LENGTH = 4;
		static const int DIFFICULTY_PER_WORD_INC = 20;

};

#endif // DICTIONARY_H