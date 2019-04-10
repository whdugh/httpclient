
/*
*date:2019-3-31
*function:for http download
*/

#include <iostream>

#include "downloadService.h"
#include "httpClient.h"

//#include "task.h"

using namespace std;

class testUnit:public DownloadTaskRepository
{
public
	void getTaskList(std::list<std::shared_ptr<DownloadTask>> & theTaskList)
	{
		
		for(int i =0 ;i < 10;++i)
		{
			std::shared_ptr<DownloadTask> task = std::make_shared<DownloadTask>();
			task.id = i;
			task.destHost("localhost");
			task.protocol = "HTTP";
			task.destPort = 0;
			task.url("http://www.baidu.com/index.html");
			theTaskList.emplace_pack(task);
		}

	}
	
}
int main(int argc,char* argv[])
{
	//httpClient aHttpClient;
	DownloadService downloadService;
	downloadService.start();
	
	return 0;
}