#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include <string>
#include "BaseRequest.h"

class httpRequest:public BaseRequest 
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