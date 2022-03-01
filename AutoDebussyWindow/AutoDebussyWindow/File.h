#pragma once

#include "HandleError.h"
#include <vector>
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

	int get_dir_all_file_name(std::vector<std::string>& rt
					, std::string& dir_name);
	bool open(const std::string& file_path, const char* mode);
	int close();
	int write(const void* datas, size_t size);
	int read(void* data, size_t size);

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


