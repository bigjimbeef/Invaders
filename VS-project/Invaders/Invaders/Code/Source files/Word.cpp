#include "Word.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"
#include "EnemyProjectile.h"

Word::Word(IRenderable& owner, const char* text, bool mammoth) :
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
		// Initialise the vert deviation to nothing.
		m_verticalDeviation[i] = 0.0f;
		m_deviationTime[i] = static_cast<float>(i);
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

	m_spriteWidth = !mammoth ? LETTER_SIZE : LETTER_SIZE * MAMMOTH_LETTER;
	m_spriteHeight = !mammoth ? LETTER_SIZE : LETTER_SIZE * MAMMOTH_LETTER;
	m_spriteClipWidth = !mammoth ? LETTER_SIZE : LETTER_SIZE * MAMMOTH_LETTER;
	m_spriteClipHeight = !mammoth ? LETTER_SIZE : LETTER_SIZE * MAMMOTH_LETTER;

	// Big letters don't turn.
	if ( mammoth )
	{
		m_rotationPerFrame = 0.0f;
	}
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
		m_position = m_owner.GetPosition();

		// Need to limit the word to the screen.
		int length = m_wordText.length() * LETTER_SPACING;
		float maxPos = static_cast<float>(Renderer::GetScreenWidth() - length);

		MathsHelper::Clamp(m_position.x, 0.0f, maxPos);

		// Move characters up and down slightly, in a wave.
		// The wave-like propagation is provided by the differeing initial
		// deviation time created in the ctor.
		for ( unsigned int i = 0; i < m_wordText.length(); ++i )
		{
			m_deviationTime[i] += 
				static_cast<float>(frameTime / WIGGLE_FREQUENCY_SCALAR);
			float deviation = 
				static_cast<float>(WIGGLE_AMPLITUDE * sin(m_deviationTime[i]));
			m_verticalDeviation[i] = deviation;
		}
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

void Word::RenderBackground()
{
	float time = GameState::GetInstance().GetTimeEducating();
	float baseTime = static_cast<float>(GameState::GetEducationDuration());

	// How much of the total time is left?
	int baseWidth = LETTER_SPACING * m_wordText.length();
	float proportion = 1.0f - ( time / baseTime );
	int width = static_cast<int>(baseWidth * proportion);

	// We render the bar in three colours depending on how much of it is left.
	float twoThirds = 0.66f;
	float oneThird = 0.33f;
	DWORD colour = Renderer::GetColour(255, 255, 255);
	colour =	
			proportion > twoThirds
			// Bright green if n > 0.66f
			? Renderer::GetColour(0, 255, 0)
			: proportion > oneThird
				// Orange if 0.33f < n < 0.66f
				? Renderer::GetColour(255, 127, 0)
				// Red if n < 0.33f
				: Renderer::GetColour(255, 0, 0);

	// Add a border to the background.
	float borderWidth = 3.0f;
	float xPos = m_position.x - borderWidth;
	float yPos = m_position.y - borderWidth;
	int bgWidth = baseWidth + static_cast<int>( borderWidth * 2 );
	int bgHeight = m_spriteHeight + static_cast<int>( borderWidth * 2 );

	Game::GetInstance().GetRenderer().DrawFilledRect(
		xPos, yPos, bgWidth, bgHeight, 
		// Darkish grey.
		Renderer::GetColour(100, 100, 100)
	);
	Game::GetInstance().GetRenderer().DrawFilledRect(
		m_position.x, m_position.y, 
		width, m_spriteHeight, colour
	);
}

void Word::Render()
{
	// If we're rendering a whole word...
	if ( m_wordText.length() > 1 )
	{
		// Render our background first.
		RenderBackground();

		Vector2 charPos = m_position;

		for ( unsigned int i = 0; i < m_wordText.length(); ++i )
		{
			// Apply vertical deviation.
			float yPos = charPos.y + m_verticalDeviation[i];

			DWORD col = Renderer::GetColour(255, 255, 255);
			if ( m_lettersCleared > i )
			{
				// Render the letter transparent if it has
				// already been entered.
				col = Renderer::GetColour(0, 0, 0, 0);
			}

			Game::GetInstance().GetRenderer().DrawSprite(
				m_textSprites[i], charPos.x, yPos, 
				m_spriteWidth, m_spriteHeight, m_rotation, col
			);

			// Space the letters out
			charPos.x += LETTER_SPACING;
		}
	}
	else
	{
		// Use the Renderer to draw the player's sprite.
		Game::GetInstance().GetRenderer().DrawSprite(
			m_textSprites[0], m_position.x, m_position.y, 
			m_spriteWidth, m_spriteHeight, m_rotation,
			Renderer::GetColour(255, 255, 255)
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

		// If it's the correct letter.
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

					// Play the letter sound effect
					Game::GetInstance().GetAudioManager().PlaySoundEffect("letter");

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
					p_owner->Kill(true);

					// Work out the new difficulty because of the death.
					GameState::GetInstance().RecalculateDifficulty();

					GameState::GetInstance().EndEducation();
				}

				return true;
			}
		}
		// If it's the wrong letter...
		else
		{
			// If we're a full word ...
			if ( !m_owner.IsEnemyProjectile() )
			{
				// ... then increment the education timer!
				GameState::GetInstance().IncTimeEducating(0.5f);
			}
		}
	}

	return false;
}
