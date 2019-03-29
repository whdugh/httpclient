
#include <errno.h>
#include "downloadService.h"

DownloadService::DownloadService()
{

}

DownloadService::~DownloadService()
{

}

size_t DownloadService::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}


int DownloadService::downloadFile(const char* url, char * filename)
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
