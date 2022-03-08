#include "Thread.h"

namespace tt {
namespace system {



Scheduler::Scheduler(uint64_t thread_num) {
	m_thread_num = thread_num;
	m_sema = new std::counting_semaphore<10000>(0);
}

Scheduler::~Scheduler() { 
	delete m_sema; 
	stop();
}

void Scheduler::add_task(Task t)
{
	std::unique_lock<std::mutex> mtx(m_mtx);
	m_tasks.push(std::move(t));
	m_sema->release();
}

void Scheduler::init()
{
	for (int i = 0; i < m_thread_num; i++) {
		m_thread_pool.push_back(std::thread(std::bind(&Scheduler::run, this)));
	}
}

void Scheduler::run()
{
	Task t;
	while (1)
	{
		while (!m_tasks.empty())
		{
			m_sema->acquire();
			{
				std::unique_lock<std::mutex> mtx(m_mtx);
				t = m_tasks.front();
				m_tasks.pop();
			}
			try { // 不加try 调试会中断
				t.cb();
			}
			catch (const std::exception& e) {
				std::cout << e.what() << std::endl;
			}
		}
	}
}

void Scheduler::stop()
{
	for (auto& it : m_thread_pool) {
		it.join();
	}
}

} // namespace system
} // namespace tt



