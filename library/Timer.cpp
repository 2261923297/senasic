#include "Timer.h"

namespace tt {
namespace system {

bool Timer::_run() {
	uint64_t now = clock();
	if (now < m_start_time) {
		__logfmt_debug("sleeping...");
		std::this_thread::sleep_for(
			std::chrono::milliseconds(
				m_start_time - clock()));
	}
	if (m_error_time < now) {
		return false;
	}
	m_cb(m_cb_args);
	return true;
}

} // namespace system
} // namespace tt
