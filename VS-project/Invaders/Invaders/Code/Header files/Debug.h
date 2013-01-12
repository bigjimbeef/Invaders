#ifndef DEBUG_H
#define DEBUG_H

//-----------------------------------------------------------------------------
// Debug.h
// Provides an interface for rendering debug information to the screen.
//-----------------------------------------------------------------------------

// Use STL map for managing debug text. Please see documentation for info.
#include <map>

// Forward declare Game to allow access to Game singleton.
class Game;

class Debug
{
	public:
		static Debug& GetInstance()
		{
			static Debug instance;
			return instance;
		}
		virtual ~Debug();

		// Render the debug text to the screen.
		void Render();

		// Push a string to the debug text renderer.
		void DrawDebugText(const char* index, const char* pValue);

	private:

		// The map of debug text used by the program.
		std::map<const char*, const char*> m_debugText;

		static const int DEBUG_TEXT_HEIGHT = 20;

		// Private default ctor to facilitate Singleton pattern.
		Debug();

		// DO NOT IMPLEMENT
		Debug(Debug const&);
		void operator=(Debug const&);
};

#endif // DEBUG_H