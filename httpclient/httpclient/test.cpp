
/*
*date:2019-3-31
*function:for http download
*/

#include <iostream>

#include "downloadService.h"
#include "httpClient.h"

//#include "task.h"

using namespace std;

int main(int argc,char* argv[])
{
	httpClient aHttpClient;
	
	DownloadTask task;
	task.id = 1;
	task.destHost("localhost");
	task.protocol = "HTTP";
	task.destPort = 0;
	task.url("http://www.baidu.com/index.html");
	
	
	DownloadService downloadService;
	
	

	return 0;
}