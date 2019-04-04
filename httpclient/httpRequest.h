#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include <string>
#include "Message.h"

class httpRequest :public Message
{
public:

public:
	httpRequest();
	~httpRequest();

	void setUrl(std::string url){ myUrl = url; }
	std::string getUrl(){ return myUrl; }


private:
	std::string myUrl;

};



#endif