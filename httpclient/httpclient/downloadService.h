#ifndef _DOWN_LOAD_SERVICE_H_
#define _DOWN_LOAD_SERVICE_H_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "httpClient.h"
#include "Message.h"
//#include "Factory.h"

using namespace std;

//#include "task.h"

/**********************************************
usage:

**********************************************/
class DownloadTask
{
public:
	int id;
	std::string destHost;
	std::string protocol;
	unsigned short destPort;
	std::string user;
	std::string password;
	std::string filename;
	std::string url;
};

class DownloadTaskRepository
{
public:
	//for extend
	virtual void getTaskList(std::list<std::shared_ptr<DownloadTask>> & theTaskList) = 0;
	
	//void getTaskList(std::list<std::shared_ptr<DownloadTask>> & theTaskList);
	
};

class DownloadService
{
public:
	DownloadService(DownloadTaskRepository& theReposity);
	virtual ~DownloadService();

	unsigned int getNumberOfThreads();
	void setNumberOfThreads(unsigned int count);

	//start download thread
	bool start();
	bool stop();
private:
	void scheduleDownloadTask(std::shared_ptr<DownloadTask> task);
	void runDownload();
	void runSchedule();

	void download(DownloadTask &task);

	std::shared_ptr<DownloadTask> getDownloadTask();

private:
	DownloadTaskRepository &myRepository;
	std::thread myScheduleThread;
	
	unsigned int myNumberOfThreads;
	std::mutex myLockTaskList; //for downtasklist
	std::condition_variable myCondition;
	std::vector<std::thread> myThreadList;
	std::list< std::shared_ptr<DownloadTask> > myDownloadTaskList;
};



#endif