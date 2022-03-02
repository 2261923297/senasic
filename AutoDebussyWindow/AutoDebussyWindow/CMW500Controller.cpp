#include "CMW500Controller.h"

namespace senasic {
namespace sw {
namespace tt {

CMW500Controller::CMW500Controller(const std::string& config_file
	, const std::string& ip
	, uint16_t port) : InstrumentController()
{
	set_config_file(config_file);
	int res = init(ip, port);
	m_config_cur = 0;
	if (res == -1) {
		exit(-1);
	}
}

// 1 success, 0 false
int CMW500Controller::set()
{
	if (m_configs.size() == 0) {
		return 0;
	}
	int rt = 1;
	std::tuple<std::string, std::string> item
		= m_configs[m_config_cur % m_configs.size()];
	m_config_cur++;
	if (m_config_cur % m_configs.size() == 0) {
		rt = 0;
	}

	std::string fre = std::get<0>(item);
	std::string levle = std::get<1>(item);
	std::string cmd_frequence
		= "SOURce:GPRF:GENerator:RFSettings:FREQuency "
		+ fre
		+ "\n";

	std::string cmd_level
		= "SOURce:GPRF:GENerator:RFSettings:LEVEl "
		+ levle
		+ "\n";
/*
	if (levle[0] == '-') {
		levle[0] = 'n';
	}
*/
	m_ad->set_file_name(fre + "_" + levle + ".txt");

	if (false == m_sock->send(cmd_frequence)) { return -1; }
	if (false == m_sock->send(cmd_level)) { return -1; }

	return rt;
}

int CMW500Controller::get()
{
	std::string rest;
	m_sock->recv(rest);
	fprintf(stdout, "CMW500 return: %s\n", rest.c_str());
	return 0;
}

int CMW500Controller::read_config_from_file(const std::string& file)
{
	FILE* pFile = fopen(file.c_str(), "rb");
	if (pFile == nullptr) {
		fprintf(stderr, "can^t open file %s\n", file.c_str());
		return -1;
	}
	fseek(pFile, SEEK_SET, 0);
	char buffer[256] = { 0 };
	std::string config;
	std::string frequence;
	std::string level;

	int sp = 0, end = 0;
	while (fscanf(pFile, "%s\n", buffer) > 0) {
		config = buffer;
		sp = config.find_first_of(',');
		end = config.size();
		if (sp == std::string::npos || end == std::string::npos) {
			fprintf(stderr, "config_file format error file: %s\n", file.c_str());
			return -1;
		}
		frequence = config.substr(0, sp);
		level = config.substr(sp + 1, end - sp - 1);
		m_configs.push_back(std::tuple<std::string, std::string>
			(frequence, level));
		memset(buffer, 0, 256);
	}
	return 0;
}


} // namespace tt
} // sw
} // namespace senasic