#pragma once

#include "HandleError.h"
#include <filesystem>

namespace senasic {
namespace system {
namespace tt {

class File;
class HandleFileError 
	: public senasic::tool::tt::HandleError {
public:
	HandleFileError() { m_file = nullptr; }

protected:
	virtual void handle() override;
	virtual void set_args(void* args) override;
private:
	File* m_file;

}; // class HandleFileError

class File {
public:
	File();		
	~File();	// free handler

	bool open(const std::string& file_path, const char* mode);
	int close();
	int write(const void* datas, size_t size);
	int read(void* data, size_t size);
	int get_all_file_in_dir(std::vector<std::string>& rt
								, const std::string& dir);
	int flush() { return fflush(m_file_pointer); }
	const char* get_cur_open() const { return m_file_addr.c_str(); }
	const char* get_cur_mode() const { return m_open_mode.c_str(); }
	
	// delete last handler
	void set_handle_error(senasic::tool::tt::HandleError* he);
private:
	senasic::tool::tt::HandleError* m_handler;
	std::string m_file_addr;
	std::string m_open_mode;
	FILE* m_file_pointer;
}; // class File


}
}
}


