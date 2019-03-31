
#ifndef _DOWN_LOAD_SERVICE_H_
#define _DOWN_LOAD_SERVICE_H_

#include <string>
#include <functional>
#include <thread>

#include <curl/curl.h>

using namespace std;

#define NUM_HANDLES 1000

struct transfer {
	CURL *easy;
	unsigned int num;
	FILE *out;
};

class httpClient
{
public:
	httpClient();
	~httpClient();

	//登录
	bool loginHttpServer();

	void setOpt(struct transfer *t, int num);

	//callback
	size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);

	int downloadFile(const char* url, char * filename);


private:
	struct transfer myTransfers[NUM_HANDLES];
};

#endif