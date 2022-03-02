#include "N900Controller.h"

#include "AutoDebussy.h"
#include "tools.h"
#include "structure.h"

#include <vector>
#include <string>
#include <stdio.h>
#include <tuple>
#include <iostream>
#include <qdebug.h>
#include <qobject.h>


namespace senasic {
namespace sw {
namespace tt {

N900Controller::N900Controller
(const std::string& config_file
	, const std::string& ip
	, uint16_t port) : InstrumentController()
{
	m_fid = 0;
	m_save_to_dir = "D:\\Auto\\Debussy\\results\\N900\\";
	m_save_to_file = std::string("Debussy_") + __TIME__;
	set_config_file(config_file);
	int res = init(ip, port);
	if (res == -1) {
		exit(-1);
	}
}

int N900Controller::read_config_from_file
(const std::string& file)
{

	return 0;
}
int N900Controller::set()
{
	m_sock->send("Hi N900, there are some settings for you!\n");
	return 0;
}

int N900Controller::get()
{
	std::string ctx;
	std::string file_name = m_save_to_dir
		+ m_save_to_file + std::to_string(m_fid);
	FILE* pFile = nullptr;
	pFile = fopen(file_name.c_str(), "ab+");
	if (pFile == nullptr) {
		fprintf(stderr, "can^t open file %s\n", file_name.c_str());
		return -1;
	}
	fseek(pFile, SEEK_END, 0);

	m_sock->send("Hi N900, give me your result!\n");
	size_t nWrite = 0;
	while (m_sock->recv(ctx)) {
		nWrite = fwrite(ctx.c_str(), 1, ctx.size(), pFile);
		if (nWrite <= 0) {
			fprintf(stderr, "fwrite <= 0\n");
			return -1;
		}
	}
	return 0;

}
} // namespace tt
} // sw
} // namespace senasic
