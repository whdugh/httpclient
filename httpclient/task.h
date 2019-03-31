#ifndef _Task_H_
#define _Task_H_

#include <thread>
#include <mutex>
#include <vector>
#include <list>
#include <deque>
#include <memory>
#include <functional>
#include <iostream>

using namespace std;

/*
* for multi thread process download,upload and so on Tasks.
*/

template<typename T>
class ThreadPool
{
public:
	typedef std::function<void(void)> task;//?

	ThreadPool()=default;
	ThreadPool(const ThreadPool& x) = delete;
	ThreadPool&operate = (const ThreadPool&x) = delete;
	~ThreadPool() =default;

	void start();

	//添加一条任务
	void addTaskList(std::shared_ptr<T>& request);
	T& popTask();

	void schedule();

private:
	int myThreadNums;	//线程池线程个数
	std::vector<std::thread> myWorks;
	std::deque<task> myTaskList;
	std::mutex myTaskListMutex;
};

template<typename T>
ThreadPool<T>::ThreadPool()
{
	//for ()
}

template<typename T>
ThreadPool<T>::~ThreadPool()
{
}

template<typename T>
void ThreadPool<T>::start()
{

}
template<typename T>
void ThreadPool<T>::addTaskList(std::shared_ptr<T>& request)
{
	std::lock_guard<std::mutex> guard<myTaskListMutex>;
	myTaskList.emplace_back(request);
}

#endif