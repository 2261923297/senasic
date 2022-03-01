#pragma once

#include <stdio.h>
#include <string>

#define STR_LINE_LOCAL(desc)					\
	(std::string(__FILE__) + ": "				\
	+ std::to_string(__LINE__)	+ ": "			\
	+ __FUNCTION__ +">"							\
	+ desc ).c_str()							\
//

namespace senasic {
namespace tool {
namespace tt {

class HandleError {
public:
	HandleError() { }
	virtual ~HandleError() { }
	virtual void set_args(void* args) = 0;

	// [ERROR] __TIME__, errdesc \n
	void handle_error(bool is_error
		, const char* err_desc
		, bool need_handle = 1) {
		if (is_error) {
			fprintf(stderr, "[ERROR] %s %s\n", __TIME__, err_desc);
			if (need_handle) {
				handle();
			}
		}
	}
protected:
	virtual void handle() = 0;

private:

}; // class HandleError



} // namespace tt
} // namespace tool
} // namespace senasic