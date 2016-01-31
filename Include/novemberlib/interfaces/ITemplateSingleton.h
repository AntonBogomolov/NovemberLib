#ifndef ITEMPLATESINGLETON_H
#define ITEMPLATESINGLETON_H

#include "IUncopyable.h"

template<class T>
class ITemplateSingleton : public IUncopyable
{
	public:
		static T* getInstance()
		{
			if(instance == 0)
			{
				instance = new T();
			}
			return instance;
		}

		virtual ~ITemplateSingleton (){};
	protected:
	private:
		static T* instance;
};

template<class T>
T* ITemplateSingleton<T>::instance = 0;

#endif // ITEMPLATESINGLETON _H
