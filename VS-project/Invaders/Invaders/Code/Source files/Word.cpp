#include "Word.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"
#include "EnemyProjectile.h"

Word::Word(IRenderable& owner, const char* text) :
	m_owner(owner),	
	m_wordText(text),
	m_lettersCleared(0)
{
	m_position = m_owner.GetPosition();

	for ( int i = 0; i < MAX_LETTERS; ++i )
	{
		m_textSprites[i] = NULL;
	}

	// If we're not being created with a specific word...
	if ( m_wordText == "" )
	{
		// ... then we use a random letter.
		char letter = GetRandomLetter();
		m_wordText += letter;

		m_textSprites[0] = 
			Game::GetInstance().GetResourceManager().GetLetterSprite(letter);
	}
	else
	{
		// TODO: init multi-letter words.
	}

	m_spriteWidth = LETTER_SIZE;
	m_spriteHeight = LETTER_SIZE;
	m_spriteWidth = LETTER_SIZE;
	m_spriteClipHeight = LETTER_SIZE;
}
Word::~Word()
{

}

char Word::GetRandomLetter()
{
	const char* alphabet = "abcdefghijklmnopqrstuvwxyz";
	int letterIdx = rand() % 26;
	char letter = alphabet[letterIdx];
	
	return letter;
}

void Word::Update(float frameTime)
{
	m_position = m_owner.GetPosition();

	int width = m_owner.GetSpriteWidth();
	int height = m_owner.GetSpriteHeight();

	m_position.x -= ((m_spriteWidth - width) /2);
	m_position.y -= ((m_spriteHeight - height) /2);
}

void Word::Render()
{
	// TODO: MOST OF THIS

	// Use the Renderer to draw the player's sprite.
	Game::GetInstance().GetRenderer().DrawSprite(
		m_textSprites[0], m_position.x, m_position.y, m_spriteWidth, m_spriteHeight
	);

}

bool Word::ReceiveLetter(char letter)
{
	int wordLength = m_wordText.length();
	
	// This should always be true, but just being safe.
	if ( m_lettersCleared < wordLength )
	{
		char currentChar = m_wordText[m_lettersCleared];

		if ( letter == currentChar )
		{
			// We've cleared a letter.
			m_lettersCleared++;

			// Again, this should always be equal, not greater, but be safe.
			if ( m_lettersCleared >= wordLength )
			{
				if ( m_owner.IsEnemyProjectile() )
				{
					EnemyProjectile* p_proj = 
						static_cast<EnemyProjectile*>(&m_owner);
					
					p_proj->Kill();
				}

				return true;
			}
		}
	}

	return false;
}
