
#include <errno.h>
#include "httpClient.h"

httpClient::httpClient()
{

}

httpClient::~httpClient()
{

}

void httpClient::setOpt(struct transfer *t, int num)
{
	char filename[128];
	CURL *hnd;

	hnd = t->easy = curl_easy_init();

	snprintf(filename, 128, "dl-%d", num);

	t->out = fopen(filename, "wb");

	/* write to this file */
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, t->out);

	/* set the same URL */
	//curl_easy_setopt(hnd, CURLOPT_URL, "https://localhost:8443/index.html");
	curl_easy_setopt(hnd, CURLOPT_URL, "http://www.baidu.com/index.html");

	/* please be verbose */
	curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(hnd, CURLOPT_DEBUGFUNCTION, my_trace);
	curl_easy_setopt(hnd, CURLOPT_DEBUGDATA, t);

	/* HTTP/2 please */
	curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);

	/* we use a self-signed test server, skip verification during debugging */
	curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);

	//fix SSL23_GET_SERVER_HELLO:unknown protocol
	curl_easy_setopt(hnd, CURLOPT_SSLVERSION, 3); //…Ë∂®SSL∞Ê±æ
	curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0);
}

size_t httpClient::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}


int httpClient::downloadFile(const char* url, char * filename)
{
	CURL *curl_handle;
	FILE *fp = nullptr;
	fp = fopen(filename, "wb");
	if (fp == nullptr)
	{
		perror("open fail");
		return -1;
	}


	CURLcode perform_ret;

	//init curl session
	curl_handle = curl_easy_init();
	if (curl_handle)
	{
		//set opt
		curl_easy_setopt(curl_handle, CURLOPT_URL, url);

		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp);

		//provide a buffer to store erros
		char errbuf[CURL_ERROR_SIZE];
		curl_easy_setopt(curl_handle, CURLOPT_ERRORBUFFER, errbuf);
		errbuf[0] = 0;

		//finish transport
		perform_ret = curl_easy_perform(curl);
		long aHTTPResCode = 0;
		CURLcode res = curl_easy_getinfo(curl_handle, CURLINFO_HTTP_CODE, &aHTTPResCode);
		if (perform_ret == CURLE_OK && aHTTPResCode != 200)
		{
			std::cout << __FUNCTION__ << ",login http failed! ret=" << perform_ret
				<< ",aHTTPResCode=" << aHTTPResCode << std::endl;
			curl_easy_cleanup(curl);
			fclose(fp);

			return -1;
		}		
	}

	curl_easy_cleanup(curl);
	fclose(fp);
	return 0;
}
