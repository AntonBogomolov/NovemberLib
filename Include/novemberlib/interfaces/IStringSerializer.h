#ifndef ISTRINGSERIALIZER_H
#define ISTRINGSERIALIZER_H

#include "../utils/config.h"

#include <string>

class IStringSerializer
{
	public:
		virtual ~IStringSerializer(){};

		virtual std::string toString() const = 0;
		virtual void fromString(std::string& str) = 0;
	protected:
	private:
};

#endif // ISTRINGSERIALIZER_H
