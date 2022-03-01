#include "File.h"
#include "tools.h"

#include <stdio.h>
#include <filesystem>
#define __DEBUG_DESC LOG_PROFIX

namespace senasic {
namespace system {
namespace tt {
void HandleFileError::handle() {
	if (m_file != nullptr)
		m_file->close();
}

void HandleFileError::set_args(void* args)
{
	m_file = (File*)args;
}
File::File()
{
	m_handler = nullptr;
	m_file_addr = "";
	m_open_mode = "null";
}
File::~File()
{
	close();
	set_handle_error(nullptr);
	free(m_handler);
}

using namespace std::filesystem;
using namespace std;

int File::get_dir_all_file_name(std::vector<std::string>& rt
	, std::string& dir_name)
{
	path str(dir_name);
	if (!exists(str))		//必须先检测目录是否存在才能使用文件入口.
		return 1;
	directory_entry entry(str);		//文件入口
	if (entry.status().type() == file_type::directory) {
		directory_iterator list(str);	        //文件入口容器
		for (auto& it : list)
			rt.push_back(it.path().filename().string());
	}
	return 0;
}

bool File::open(const std::string& file_path, const char* mode)
{
	close();
	m_file_pointer = ::fopen(file_path.c_str(), mode);
	m_handler->handle_error(m_file_pointer == nullptr,
		(__DEBUG_DESC).c_str());
	return m_file_pointer != nullptr;
}

int File::close()
{
	int rt = 0;
	if (m_file_pointer != nullptr) {
		rt = ::fclose(m_file_pointer);
		m_file_pointer = nullptr;
	}
	m_open_mode = "null";
	m_file_addr = "";

	return rt;
}
int File::write(const void* datas, size_t size)
{
	int ret = fwrite(datas, 1, size, m_file_pointer);
	m_handler->handle_error(ret == -1 || ret != size,
		(__DEBUG_DESC).c_str());
	return ret;
}
int File::read(void* data, size_t size)
{
	int ret = fread(data, 1, size, m_file_pointer);
	m_handler->handle_error(ret == -1 || ret != size,
		(__DEBUG_DESC).c_str());
	return ret;
}
void File::set_handle_error(senasic::tool::tt::HandleError* he) {
	if (he != nullptr)
		delete he;
	m_handler = he;
	if (nullptr != m_handler)
		m_handler->set_args(this);
}
}

}
}