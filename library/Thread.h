#pragma once

#include "Mutex.h"
#include <thread>
#include <future>
#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <stdexcept>
#include <semaphore>

namespace tt {
namespace system {

struct Task {
	std::function<void()> cb;
};

class Scheduler {
public:
	using ptr = std::shared_ptr<Scheduler>;

	Scheduler(uint64_t thread_num);
	~Scheduler();

	void add_task(Task t);
	void init();
	void run();
	void stop();

public:
	
private:
	uint64_t m_thread_num;

	std::vector<std::thread> m_thread_pool;
	std::queue<Task> m_tasks;

	std::mutex m_mtx;
	std::counting_semaphore<10000>* m_sema;
}; // class Scheduler

} // namespace system
} // namespace tt
