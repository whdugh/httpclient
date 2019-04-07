#ifndef _FACTORY_H_
#define _FACTORY_H_
#include "Message.h"
#include "httpClient.h"

class Message;

class Factory
{
public:
	Factory();
	~Factory();
	
	Message* create(std::string& protocolType);
private:
	Message *myAbstractMessage;
};



#endif