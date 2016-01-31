#ifndef IJSONSERIALIZER_H
#define IJSONSERIALIZER_H

#include "../utils/config.h"

#include <string>

class IJSONSerializer
{
	public:
		virtual std::string toJSON() const = 0;
		virtual void fromJSON(std::string& str) = 0;

		virtual ~IJSONSerializer();
	protected:
	private:
};

#endif // IJSONSERIALIZER_H
