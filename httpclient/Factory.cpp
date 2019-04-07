#include "Factory.h"


Message* Factory::create(std::string& protocolType)
{
	if (protocolType == "HTTP")
	{
		myAbstractMessage =  new httpClient();
	}
	
	return myAbstractMessage;
}
