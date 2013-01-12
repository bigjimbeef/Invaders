#ifndef SINGLETON_H
#define SINGLETON_H

//-----------------------------------------------------------------------------
// Singleton.h 
// This base class provides access to the Singleton design pattern for use in 
// various parts of the codebase. This is discussed in greater detail in the 
// attached documentation.
//-----------------------------------------------------------------------------

template <typename T>
class ISingleton
{
	public:
		static T& GetInstance();
        
		virtual ~ISingleton();

	protected:
		ISingleton();

		// DO NOT IMPLEMENT
		ISingleton(ISingleton const&);
		void operator=(ISingleton const&);
};

template <typename T>
T& ISingleton<T>::GetInstance()
{
	static T instance;
	return instance;
}

#endif // SINGLETON_H