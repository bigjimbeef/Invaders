#include "Debug.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

Debug::Debug()
{
}
Debug::~Debug()
{
	std::map<const char*, const char*>::iterator it = m_debugText.begin();
	for ( it; it != m_debugText.end(); ++it )
	{
		// Tidy up the strings.
		delete it->second;
	}

	// Destroy the STL container.
	m_debugText.clear();
}

void Debug::DrawDebugText(const char* index, const char* value)
{
	// Insert the item into the map.
	if ( m_debugText.find(index) == m_debugText.end() )
	{
		m_debugText[index] = value;
	}
	else
	{
		
	}
}

void Debug::Render()
{
	std::map<const char*, const char*>::iterator it = m_debugText.begin();

	int xPos = 0;
	int yPos = 0;

	for ( it; it != m_debugText.end(); ++it )
	{
		// Draw the value of the debug text.
		Game::GetInstance().GetSystem().drawText(xPos, yPos, it->second);

		// Increment the y position of the text being drawn
		yPos += DEBUG_TEXT_HEIGHT;
	}
}