#ifndef SINGLETON_HPP
#define SINGLETON_HPP

//==========================================================================|
//								 Singleton									|
//==========================================================================|
// @brief: Singleton Template class to be used when only one instance		|
//		   of the object should exist in the program						|
//==========================================================================|

template <class T>
class Singleton
{
public:
	Singleton(){}
	virtual ~Singleton(){}
	Singleton(const Singleton &singleton) = delete;
	Singleton(Singleton &&singleton) = delete;
	Singleton &operator=(const Singleton &singleton) = delete;
	Singleton &operator=(Singleton &&singleton) = delete;

	static T *instance()
	{
		static T *instance = new T();
		return instance;
	}
};

#endif