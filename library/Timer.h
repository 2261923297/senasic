#pragma once
#include <chrono>
#include <functional>
#include <thread>


#include "utils.h"
#include "HandleError.h"

namespace tt {
namespace system {



class Timer {
public:
	using cb_t = std::function<void(void*)>;
	Timer(uint32_t id, uint64_t start_time
		, cb_t cb, void* args = nullptr
		, uint64_t error_time = -1) 

	:	m_id(id), m_start_time(start_time)
		, m_cb(cb), m_cb_args(args)
		, m_error_time(error_time) {
		
	}
	void set_handle_error(senasic::tool::tt::HandleError* he) {
		m_he = he;
		if (m_he != nullptr) {
			m_he->set_args(this);
		}
	}
	void run() { m_he->handle_error(_run() == false, (LOG_PROFIX).c_str(), false); }
	bool _run();
	uint64_t get_wait_time() { return m_start_time - clock(); }

private:
	uint32_t m_id;
	uint64_t m_start_time;
	uint64_t m_error_time;

	cb_t m_cb;
	void* m_cb_args;
	senasic::tool::tt::HandleError* m_he;
}; // class Timer

} // namespace system
} // namespace tt

