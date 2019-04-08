#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>

class Message
{
public:
	Message();
	virtual ~Message();

	//create a object
	//Message* create(std::string & protocolType);

	//download data
	virtual int get(const std::string& url, std::string& filename, bool secure = false);

private:

};

#endif