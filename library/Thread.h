#pragma once

#include <mutex>
#include <thread>
#include <queue>

namespace tt {
namespace system {

struct task {

}; // struct task;
template<class T>
class ThrreadSafeQueue {
public:
	bool empty();
	size_t size();

	void push(T& t);
	bool pop(T& t);

private:
	std::mutex    m_mutex;
	std::queue<T> m_que;
};


class ThreadPool
{
public:
	ThreadPool(size_t pool_size);
protected:
	void run();
private:
	std::vector<std::thread>		m_thrd;

}; // class Thread


template<class T>
inline bool ThrreadSafeQueue<T>::empty()
{
	m_mutex.lock();
	return m_que.empty();
	m_mutex.unlock();
}

template<class T>
inline size_t ThrreadSafeQueue<T>::size()
{

	m_mutex.lock();
	return m_que.size();
	m_mutex.unlock();
}

template<class T>
inline void ThrreadSafeQueue<T>::push(T& t)
{
	m_mutex.lock();
	m_que.push(t);
	m_mutex.unlock();
}

template<class T>
inline bool ThrreadSafeQueue<T>::pop(T& t)
{
	if (empty) {
		return false;
	}
	m_mutex.lock();

	t = m_que.front();
	m_que.pop();

	m_mutex.unlock();
	return true;
}

} // namespace system
} // namespace tt
