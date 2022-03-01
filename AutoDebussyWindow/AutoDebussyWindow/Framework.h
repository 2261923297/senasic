#pragma once

#include "Socket.h"

#include <memory>
#include <string>
#include <vector>
#include <list>

namespace tt {
namespace framework {

class InstrumentController {
public:
	using ptr = std::shared_ptr<InstrumentController>;

	InstrumentController();
	virtual ~InstrumentController() {
		printf("InstrumentController delete\n\n");
	}

	// read_config_from_file socket->connect  -1 error
	int init(const std::string& ip, uint16_t port = 5025);
	void set_config_file(const std::string& cf) { m_config_file = cf; }

	virtual int set() = 0;
	virtual int get() = 0;
	virtual int read_config_from_file(const std::string& file) = 0;
	virtual int get_cur_config() { return 1; }
protected:
	senasic::system::tt::Socket::ptr	m_sock;
	std::string							m_config_file;
};



class AutoDebussy {
public:
	AutoDebussy();
	~AutoDebussy();
	
	int init();
	int work();
	int tpl_friday();
private:
	InstrumentController* m_N900_ctl;
	InstrumentController* m_CMW500_ctl;

	InstrumentController* m_board_ctl;

}; // class AutoDebussy


}
}


