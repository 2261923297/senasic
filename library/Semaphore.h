#pragma once

#include <semaphore>
#include <memory>

#ifndef SEMAPHORE_MAX_COUNT
#define SEMAPHORE_MAX_COUNT 1000000
#endif // ! SEMAPHORE_MAX_COUNT

namespace tt {
namespace system {

class Semaphore {
public:
	using ptr = std::shared_ptr<Semaphore>;
	Semaphore(int sema_default_num = 0) { 
		m_sem = new std::counting_semaphore<SEMAPHORE_MAX_COUNT>(sema_default_num);
	}
	~Semaphore() { delete m_sem; }

	void wait() { m_sem->acquire(); }
	void post() { m_sem->release(); }
private:
	std::counting_semaphore<SEMAPHORE_MAX_COUNT>* m_sem;
}; // class Semaphore
}
}


