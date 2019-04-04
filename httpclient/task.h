#pragma once
#include <stdexcept>
#include <deque>
#include <thread>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <future>
#include <memory>

/******************************************************
usage:



*******************************************************/
template<typename T>
class TaskExecutor final
{
public:
	using Handler = std::function<void(T&)>;

	TaskExecutor(size_t theThreadNum = std::thread::hardware_concurrency())
		:myThreadNum(theThreadNum){

	}

	~TaskExecutor() = default;

	void start()
	{
		isStopped = false;
		for (size_t aPos = 0; aPos < myThreadNum; aPos++)
		{
			myWorkers.emplace_back(std::bind(&TaskExecutor<T>::run, this));
		}
	}

	void stop()
	{
		std::call_once(myFlag, [this]() {
			{
				std::lock_guard<std::mutex> aLock(myMutex);
				isStopped = true;
				myCond.notify_all();
			}

			for (auto & aWoker : myWorkers)
			{
				aWoker.join();
			}

			{
				std::lock_guard<std::mutex> aLock(myMutex);
				if (myPromise)
				{
					myPromise->set_value();
					myPromise.reset(nullptr);
				}
			}
		});
	}

	void gracefullyStop()
	{
		waitUncompletedTask();
		stop();
	}

	void add(T task)
	{
		{
			std::lock_guard<std::mutex> aLock(myMutex);
			if (isStopped)
			{
				return;
			}
			else
			{
				myTasks.emplace_back(std::move(task));
			}
		}
		myCond.notify_one();
	}

	T get()
	{
		std::lock_guard<std::mutex> aLock(myMutex);
		if (myTasks.empty())
		{
			throw std::out_of_range("Queue is empty.");
		}
		else
		{
			return take();
		}
	}

	bool empty()
	{
		std::lock_guard<std::mutex> aLock(myMutex);
		return myTasks.empty();
	}

	void setHandler(Handler theHandle)
	{
		myHandler = std::move(theHandle);
	}
private:
	T take()
	{
		T aTask = myTasks.front();
		myTasks.pop_front();
		return aTask;
	}

	void run()
	{
		while (!isStopped)
		{
			T aTask;
			{
				std::unique_lock<std::mutex> aLock(myMutex);
				myCond.wait(aLock, [this](){return (!myTasks.empty() || isStopped || myPromise); });

				if (isStopped)
				{
					continue;
				}

				if (myTasks.empty() && myPromise)
				{
					myPromise->set_value();
					myPromise.reset(nullptr);
					continue;
				}

				aTask = take();
			}

			if (myHandler)
			{
				try
				{
					myHandler(aTask);
				}
				catch (...)
				{
					std::cout << "TaskExecutor catch an exception." << std::endl;
				}
			}
		}
	}

	void waitUncompletedTask()
	{
		{
			std::lock_guard<std::mutex> aLock(myMutex);
			if (!myTasks.empty() && !myPromise)
			{
				myPromise.reset(new std::promise<void>());
			}
		}

		if (myPromise)
		{
			myPromise->get_future().get();
		}
	}
	 
	size_t myThreadNum;
	mutable std::mutex myMutex;
	std::condition_variable myCond;
	Handler myHandler;
	std::atomic_bool isStopped{ true };
	std::once_flag myFlag;
	std::unique_ptr<std::promise<void>> myPromise;
	std::deque<T> myTasks;
	std::deque<std::thread> myWorkers;
};
