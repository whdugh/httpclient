
#ifndef _DOWN_LOAD_SERVICE_H_
#define _DOWN_LOAD_SERVICE_H_

#include <string>
#include <functional>
#include <thread>

#include <curl/curl.h>

using namespace std;

class DownloadService
{
public:
	DownloadService();
	~DownloadService();

	//登录
	bool loginHttpServer();

	//callback
	size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);

	int downloadFile(const char* url, char * filename);


private:

};

#endif