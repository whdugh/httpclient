
/*
*date:2019-3-31
*function:for http download
*/

#include <iostream>
#include <stdio.h>
#include "downloadService.h"
#include "httpClient.h"

//#include "task.h"

using namespace std;

class testUnit:public DownloadTaskRepository
{
public:
	void getTaskList(std::list<std::shared_ptr<DownloadTask>> & theTaskList)
	{
		
		for(int i =0 ;i < 2;++i)
		{
			std::shared_ptr<DownloadTask> task = std::make_shared<DownloadTask>();
			char filename[128];
			snprintf(filename, 128, "./file-%d", i);
			task->id = i;
			task->destHost= "localhost";
			task->protocol = "HTTP";
			task->destPort = 0;
			task->url = "http://www.baidu.com/index.html";
			task->filename = filename;
			theTaskList.emplace_back(task);
		}

	}
	
};

int main(int argc,char* argv[])
{
	//httpClient aHttpClient;
	DownloadTaskRepository *aReposity = new testUnit();
	DownloadService downloadService(*aReposity);
	
	downloadService.start();
	downloadService.stop();
	return 0;
}