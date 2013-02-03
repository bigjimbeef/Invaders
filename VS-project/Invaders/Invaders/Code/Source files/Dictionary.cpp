#include "Dictionary.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

Dictionary::Dictionary() :
	m_filePath("Code/Resource files/dictionary.txt")
{
	m_wordMap = std::multimap<int, std::string>();

	// Read the dictionary into the word map.
	ReadDictionary();
}
Dictionary::~Dictionary()
{

}

void Dictionary::ReadDictionary()
{
	std::string line = "";

	m_inputFile.open(m_filePath);
	if ( m_inputFile.is_open() )
	{
		while( std::getline(m_inputFile, line) )
		{
			// Process this line.
			int length = line.length();
			m_wordMap.insert(std::pair<int, std::string>(length, line));
		}
	}
	// Close our dictionary file.
	m_inputFile.close();
}

std::string Dictionary::GetRandomWord()
{
	// Check the current difficulty.
	
	// TODO: GET DIFFICULTY
	//int difficulty = GameState::GetInstance().GetDifficulty();
	int difficulty = 60;

	int maxLetters = BASE_WORD_LENGTH + (difficulty / DIFFICULTY_PER_WORD_INC);

	float diffProgress = 
		static_cast<float>(difficulty / DIFFICULTY_PER_WORD_INC);

	/*
	 * Maximum value is 2^n, where n is maxLetters - BASE_WORD_LENGTH.
	 * We then pick a value from 0 to 2^n, and use log_2 to calc the x value
	 * to which this corresponds (i.e. the number of letters more than base).
	 */
	float maxRandVal = pow(2.0f, maxLetters - BASE_WORD_LENGTH);

	// Now, choose a random float in the region.
	float randomY = MathsHelper::randf(maxRandVal);

	// Calculate log2 as such:
	// log2 (x) = logy (x) / logy (2)
	int extraLetters = static_cast<int>(log10(randomY) / log10(2.0f));
	extraLetters = max(extraLetters, 0);
	// This is the number of extra letters beyond the base.
	int xVal = BASE_WORD_LENGTH + extraLetters;

	// Now, with our weighted random x value, get a word by calculating
	// the number of entries with this number of letters and getting a
	// random one.
	std::pair<std::multimap<int, std::string>::iterator,
			  std::multimap<int, std::string>::iterator> range;
	range = m_wordMap.equal_range(xVal);
	
	int rangeSize = std::distance(range.first, range.second);
	int randomOffset = rand() % rangeSize;
	// Advance the first iterator to the random element.
	std::advance(range.first, randomOffset);

	// This is the random string.
	std::string randomWord = range.first->second;

	return randomWord;
}