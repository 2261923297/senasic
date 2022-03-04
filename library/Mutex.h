#pragma once

#include "ClassAttributes.h"
#include <mutex>

namespace tt {
namespace system {
template <class Locker>
class MutexImpl : public tt::attr::Nocopyable
{
public:
	MutexImpl(Locker& locker)
		: m_locker(locker) {
		lock();
	}
	~MutexImpl() { unlock(); }

	void lock() {
		m_locker.lock();
	}
	void unlock() {
		m_locker.unlock();
	}
private:
	Locker& m_locker;
};

class StdMutex {
public:
	typedef MutexImpl<StdMutex> Locker;
	StdMutex() {  }
	~StdMutex() {  }

	void lock()	  { m_mutex.lock(); }
	void unlock() { m_mutex.unlock(); }
private:
	std::mutex m_mutex;
};


} // namespace thread

} // namespcae tt
