
#include "downloadService.h"


DownloadService::DownloadService(DownloadTaskRepository& theReposity) :
	myRepository(theReposity),
	myNumberOfThreads(32)
{
}

DownloadService::~DownloadService()
{
}

bool DownloadService::start()
{
	myThreadList.reserve(myNumberOfThreads);
	for (auto i = 0; i < myNumberOfThreads;++i)
	{
		myThreadList.emplace_back(std::thread(std::bind(&DownloadService::runDownload, this)));
	}
}

bool DownloadService::stop()
{
	for (auto& thread:myThreadList)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}
	myDownloadTaskList.clear();

}
void DownloadService::scheduleDownloadTask(std::shared_ptr<DownloadTask> task)
{
	std::lock_guard<std::mutex> guard(myLockTaskList);
	myDownloadTaskList.emplace_back(task);

	if (!myDownloadTaskList.empty())
		myCondition.notify_one();
}

std::shared_ptr<DownloadTask> DownloadService::getDownloadTask()
{
	std::unique_lock<std::mutex> guard(myLockTaskList);
	myCondition.wait(guard, [this](){
		return !myDownloadTaskList.empty();
	});

	std::shared_ptr<DownloadTask> task = *myDownloadTaskList.begin();
	myDownloadTaskList.pop_front();
	return task;
}

void DownloadService::runDownload()
{
	for (;;)
	{
		std::shared_ptr<DownloadTask> task = getDownloadTask();
		if (!task) break;

		download(*task.get());
	}
}

void DownloadService::download(DownloadTask &task)
{
	Factory aFactory;
	Message* message = aFactory.create(task.protocol);
	message->get(task.url,task.filename);
}