#include "Word.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"
#include "EnemyProjectile.h"

Word::Word(IRenderable& owner, const char* text) :
	m_owner(owner),	
	m_wordText(text),
	m_lettersCleared(0),
	m_rotation(0.0f),
	m_rotationPerFrame(0.0f)
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

		// Get a random rotation, between sensible min/max.
		float randRot = MathsHelper::randf(0.1f, 0.01f);
		// Generate -1 or 1 for spin direction.
		int randDir = ( rand() % 2 ) == 0 ? -1 : 1;
		m_rotationPerFrame = randDir * randRot;
	}
	else
	{
		for ( unsigned int i = 0; i < m_wordText.length(); ++i )
		{
			char letter = m_wordText[i];
			m_textSprites[i] = 
				Game::GetInstance().GetResourceManager().GetLetterSprite(letter);
		}
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
	if ( m_wordText.length() > 1 )
	{

	}
	else
	{
		m_position = m_owner.GetPosition();

		int width = m_owner.GetSpriteWidth();
		int height = m_owner.GetSpriteHeight();

		m_position.x -= ((m_spriteWidth - width) /2);
		m_position.y -= ((m_spriteHeight - height) /2);

		// Increase our rotation.
		m_rotation += m_rotationPerFrame;
	}
}

void Word::Render()
{
	// If we're rendering a whole word...
	if ( m_wordText.length() > 1 )
	{
		Vector2 charPos = m_position;

		for ( unsigned int i = 0; i < m_wordText.length(); ++i )
		{
			DWORD col = Renderer::GetColour(255, 255, 255);
			if ( m_lettersCleared > i )
			{
				col = Renderer::GetColour(0, 255, 0, 200);
			}

			// Space the letters out
			charPos.x += LETTER_SPACING;

			Game::GetInstance().GetRenderer().DrawSprite(
				m_textSprites[i], charPos.x, charPos.y, 
				m_spriteWidth, m_spriteHeight, m_rotation, col
			);
		}
	}
	else
	{
		// Use the Renderer to draw the player's sprite.
		Game::GetInstance().GetRenderer().DrawSprite(
			m_textSprites[0], m_position.x, m_position.y, 
			m_spriteWidth, m_spriteHeight, m_rotation
		);
	}
}

bool Word::ReceiveLetter(char letter)
{
	unsigned int wordLength = m_wordText.length();
	
	// This should always be true, but just being safe.
	if ( m_lettersCleared < wordLength )
	{
		char currentChar = m_wordText[m_lettersCleared];

		if ( letter == currentChar )
		{
			// Add the score for each letter
			if ( !m_owner.IsEnemyProjectile() )
			{
				Enemy* p_owner = static_cast<Enemy*>(&m_owner);

				// Add the score for this letter at the point where the
				// letter was.
				float xPos = m_position.x 
					+ (m_lettersCleared * LETTER_SPACING);
				Vector2 pos(xPos, m_position.y);

				// Add a MovingScore to the screen.
				int score = p_owner->GetScore();
				Game::GetInstance().GetRenderer().AddScoreText(
					score, pos
				);
			}

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

					// Add a MovingScore to the screen.
					Game::GetInstance().GetRenderer().AddScoreText(
						50, m_position
					);
				}
				else
				{
					// We have cleared the word. Remove the invader,
					// and transition out of education mode.
					Enemy* p_owner = static_cast<Enemy*>(&m_owner);
					p_owner->Kill();

					GameState::GetInstance().EndEducation();
				}

				return true;
			}
		}
	}

	return false;
}
