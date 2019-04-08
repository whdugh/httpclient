#ifndef _FACTORY_H_
#define _FACTORY_H_
#include <string>

#include "Message.h"
//#include "httpClient.h"

class Message;

class Factory
{
public:
	Factory(){}; //add {} for complie error: undefined reference to `Factory::Factory()' 
	~Factory(){};
	
	Message* create(std::string& protocolType);
private:
	Message *myAbstractMessage;
};



#endif