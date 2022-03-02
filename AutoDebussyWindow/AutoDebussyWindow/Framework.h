#pragma once

#include "Socket.h"

#include <memory>
#include <string>
#include <vector>
#include <list>

static std::string g_rest_root = "D:\\Auto\\Debussy\\results\\Board\\";




class AutoDebussy;
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
	
	virtual void set_ad(AutoDebussy* pAd) { m_ad = pAd; }

	virtual int set() = 0;
	virtual int get() = 0;
	virtual int read_config_from_file(const std::string& file) = 0;
	virtual int get_cur_config() { return 1; }
protected:
	senasic::system::tt::Socket::ptr	m_sock;
	std::string							m_config_file;
	AutoDebussy* m_ad;
};

class AutoDebussy {
public:
	AutoDebussy();
	~AutoDebussy();
	
	int init();
	int work();
	int tpl_friday();
	void set_save_file_name(const std::string& name) { m_save_file_name = name; }
	void set_file_name(const std::string& name) { m_file_name = name; }

	const std::string& get_sava_file_name() const { return m_save_file_name + m_file_name; }
	
private:
	InstrumentController* m_N900_ctl;
	InstrumentController* m_CMW500_ctl;
	InstrumentController* m_board_ctl;
public:
	std::string m_save_file_name;
	std::string m_file_name;
}; // class AutoDebussy


class HandleInstrumentError :
	public senasic::system::tt::HandleError {
public:
	virtual void set_args(void* args)
	{
		m_inst_ctl = (InstrumentController*)args;
	}

protected:
	virtual void handle() {  }
private:
	InstrumentController* m_inst_ctl;
};





