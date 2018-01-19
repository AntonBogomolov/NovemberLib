#ifndef IUNCOPYABLE_H
#define IUNCOPYABLE_H

#include "../utils/config.h"

class IUncopyable
{
	public:
		IUncopyable(){};
		virtual ~IUncopyable(){};
	protected:
	private:
		IUncopyable(const IUncopyable&) = delete;
		IUncopyable& operator=(const IUncopyable&) = delete;
};

#endif // IUNCOPYABLE_H
