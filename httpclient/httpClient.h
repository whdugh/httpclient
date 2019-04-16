
#ifndef _HTTP__CLIENT_H_
#define _HTTP__CLIENT_H_

#include <string>
#include <functional>
#include <thread>
#include <iostream>
#include <curl/curl.h>
#include <memory>
#include "Message.h"

using namespace std;

#define NUM_HANDLES 1000

struct transfer {
	CURL *easy;
	unsigned int num;
	FILE *out;
};

class httpClient:public Message
{
public:
	httpClient();
	~httpClient();

	//登录
	//bool loginHttpServer();
	//for debug
	static void dump(const char *text, int num, unsigned char *ptr, size_t size,char nohex);

	void setOpt(struct transfer *t, int num);

	//IF SET DEBUG INFO YOU CAN SEE MORE INFORMATION 
	void SetIsDebug(bool isDebug){ myisDebug = isDebug; }

	//callback
	//size_t static write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);

	
	//url:输入参数,请求的url(http://www.badidu.com)
	//filename:输出参数，返回的内容
	//secure:是否带证书
	int get(const std::string& url, std::string& filename, bool secure = false);

	int post(const std::string& url, const std::string& data, std::string& reponsedata, bool secure = false);

	//get remote file size
	long getRemoteFileSize(const std::string& url);

private:
	//struct transfer myTransfers[NUM_HANDLES];
	bool myisDebug;
	struct transfer *myTransfer;
	long myDownloadFileLenth;
};

#endif