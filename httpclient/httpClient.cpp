
#include <errno.h>
#include "httpClient.h"

httpClient::httpClient() :
			myisDebug(false)
{

}

httpClient::~httpClient()
{

}

//static void dump(const char *text, int num, unsigned char *ptr, size_t size,char nohex)
//{
//	size_t i;
//	size_t c;
//
//	unsigned int width = 0x10;
//
//	if (nohex)
//		/* without the hex output, we can fit more on screen */
//		width = 0x40;
//
//	fprintf(stderr, "%d %s, %lu bytes (0x%lx)\n",
//		num, text, (unsigned long)size, (unsigned long)size);
//
//	for (i = 0; i<size; i += width) {
//
//		fprintf(stderr, "%4.4lx: ", (unsigned long)i);
//
//		if (!nohex) {
//			/* hex not disabled, show it */
//			for (c = 0; c < width; c++)
//			if (i + c < size)
//				fprintf(stderr, "%02x ", ptr[i + c]);
//			else
//				fputs("   ", stderr);
//		}
//
//		for (c = 0; (c < width) && (i + c < size); c++) {
//			/* check for 0D0A; if found, skip past and start a new line of output */
//			if (nohex && (i + c + 1 < size) && ptr[i + c] == 0x0D &&
//				ptr[i + c + 1] == 0x0A) {
//				i += (c + 2 - width);
//				break;
//			}
//			fprintf(stderr, "%c",
//				(ptr[i + c] >= 0x20) && (ptr[i + c]<0x80) ? ptr[i + c] : '.');
//			/* check again for 0D0A, to avoid an extra \n if it's at width */
//			if (nohex && (i + c + 2 < size) && ptr[i + c + 1] == 0x0D &&
//				ptr[i + c + 2] == 0x0A) {
//				i += (c + 3 - width);
//				break;
//			}
//		}
//		fputc('\n', stderr); /* newline */
//	}
//}
//
//static int my_trace(CURL *handle, curl_infotype type,char *data, size_t size,void *userp)
//{
//	const char *text;
//	struct transfer *t = (struct transfer *)userp;
//	unsigned int num = t->num;
//	(void)handle; /* prevent compiler warning */
//
//	switch (type) {
//	case CURLINFO_TEXT:
//		fprintf(stderr, "== %d Info: %s", num, data);
//		/* FALLTHROUGH */
//	default: /* in case a new one is introduced to shock us */
//		return 0;
//
//	case CURLINFO_HEADER_OUT:
//		text = "=> Send header";
//		break;
//	case CURLINFO_DATA_OUT:
//		text = "=> Send data";
//		break;
//	case CURLINFO_SSL_DATA_OUT:
//		text = "=> Send SSL data";
//		break;
//	case CURLINFO_HEADER_IN:
//		text = "<= Recv header";
//		break;
//	case CURLINFO_DATA_IN:
//		text = "<= Recv data";
//		break;
//	case CURLINFO_SSL_DATA_IN:
//		text = "<= Recv SSL data";
//		break;
//	}
//
//	dump(text, num, (unsigned char *)data, size, 1);
//	return 0;
//}

//void httpClient::setOpt(struct transfer *t, int num)
//{
//	char filename[128];
//	CURL *hnd;
//
//	hnd = t->easy = curl_easy_init();
//
//	snprintf(filename, 128, "dl-%d", num);
//
//	t->out = fopen(filename, "wb");
//
//	/* write to this file */
//	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, t->out);
//
//	/* set the same URL */
//	//curl_easy_setopt(hnd, CURLOPT_URL, "https://localhost:8443/index.html");
//	curl_easy_setopt(hnd, CURLOPT_URL, "http://www.baidu.com/index.html");
//
//	/* please be verbose */
//	if (myisDebug)
//	{
//		curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
//		curl_easy_setopt(hnd, CURLOPT_DEBUGFUNCTION, my_trace);
//		curl_easy_setopt(hnd, CURLOPT_DEBUGDATA, t);
//	}
//	
//
//	/* HTTP/2 please */
//	curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
//
//	/* we use a self-signed test server, skip verification during debugging */
//	curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
//	curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
//
//	//fix SSL23_GET_SERVER_HELLO:unknown protocol
//	curl_easy_setopt(hnd, CURLOPT_SSLVERSION, 3); //…Ë∂®SSL∞Ê±æ
//	curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0);
//}

size_t httpClient::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

int httpClient::get(const std::string& url, std::string& filename, bool secure)
{
	FILE *fp = nullptr;
	fp = fopen(filename.c_str(), "wb");
	if (fp == nullptr)
	{
		perror("open fail");
		return -1;
	}

	CURLcode perform_ret;
	CURL *curl = curl_easy_init();
	if (curl == nullptr) return CURLE_FAILED_INIT;

	//set opt
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, std::bind(&httpClient::write_data,this));
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

	if (myisDebug)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		//curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
		//curl_easy_setopt(curl, CURLOPT_DEBUGDATA, t);
	}

	if (secure)
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
	}
	else
	{
		curl_easy_setopt(curl, CURLOPT_SSLVERSION, 3);//fix for ssl verson
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	}

	//provide a buffer to store erros
	char errbuf[CURL_ERROR_SIZE];
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
	errbuf[0] = 0;

	//finish transport
	perform_ret = curl_easy_perform(curl);
	long aHTTPResCode = 0;
	CURLcode res = curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &aHTTPResCode);
	if (perform_ret == CURLE_OK && aHTTPResCode != 200)
	{
		std::cout << __FUNCTION__ << ",login http failed! ret=" << perform_ret
			<< ",aHTTPResCode=" << aHTTPResCode << std::endl;
		curl_easy_cleanup(curl);
		fclose(fp);

		return -1;
	}

	curl_easy_cleanup(curl);
	fclose(fp);
	return 0;
}

