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

namespace tt {
namespace system {

class Scheduler {
public:

private:
	std::vector<std::thread> m_thread_pool;

}; // class Scheduler

} // namespace system
} // namespace tt
