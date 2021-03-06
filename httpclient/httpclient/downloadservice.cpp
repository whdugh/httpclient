#include "Factory.h"
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
	
	myScheduleThread = std::thread(std::bind(&DownloadService::runSchedule,this));
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
	
	if (myScheduleThread.joinable())
	{
		myScheduleThread.join();
	}
	myDownloadTaskList.clear();

}

void DownloadService::runSchedule()
{
	//for(;;)
	{
		std::list< std::shared_ptr<DownloadTask> > taskList;
		
		myRepository.getTaskList(taskList);
		
		for(auto task:taskList)
		{
			try
			{
				scheduleDownloadTask(task);
			}
			catch(...)
			{
				std::cout<< "schedule download task " << task->id << " failure" << std::endl;
			}
		}
	}
}

void DownloadService::scheduleDownloadTask(std::shared_ptr<DownloadTask> task)
{
	//cout <<__LINE__<<","<<__FUNCTION__<<",enter"<<endl;
	std::lock_guard<std::mutex> guard(myLockTaskList);
	myDownloadTaskList.emplace_back(task);

	if (!myDownloadTaskList.empty())
		myCondition.notify_one();
}

std::shared_ptr<DownloadTask> DownloadService::getDownloadTask()
{
	//cout <<__LINE__<<","<<__FUNCTION__<<",enter"<<endl;
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
	//cout <<__LINE__<<","<<__FUNCTION__<<",enter"<<endl;
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
	bool bFinished = message->get(task.url,task.filename);
	if(!bFinished)
	cout<<__FUNCTION__<<",file = "<<task.filename << "down finished."<<endl;
}