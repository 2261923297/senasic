#pragma once
#include "Framework.h"
#include "Socket.h" 
#include "structure.h"
#include "HandleComFrame.h"
#include "CMW500Controller.h"
#include "N900Controller.h"
#include "tools.h"

#include <memory>  
#include <vector> 
#include <list> 

#include <qserialport.h>
#include <QtSerialPort/QSerialPortInfo>

 

namespace senasic {
namespace sw {
namespace tt {
// 15200
class BoardController : public InstrumentController, public QObject {
public:
	typedef std::shared_ptr<BoardController> ptr;
	using config_t = std::tuple<std::string, std::vector<QByteArray>>;

	BoardController(const std::string& config_file, const std::string& board_com);

	virtual ~BoardController();

	virtual int get_cur_config() override;
	virtual int set() override;
	virtual int get() override;


	int get_ok(uint8_t len, const uint8_t* data_beg);
	int get_data(uint8_t len, const uint8_t* data_beg);
	void set_wait_time(uint32_t time = 100) { m_wait_time = time; }
	void set_cur_config(const config_t& config) { m_curr_config = config; }
	
	void set_ad(AutoDebussy* ad) override { 
		__logger_debug << str_val(ad) << "\n";
		m_ad = ad;
		m_hcf.set_ad(ad); 
	}

	void test_set();
	int parse_frame(const uint8_t* frame, size_t len);
	void print_configs();
	virtual int read_config_from_file
	(const std::string& file) override;
	// virtual int get_cur_config() override;
private:
	config_t framing(const char* buf);

private:
	QSerialPort* m_com;
	QByteArray		m_read_buf = { 0 };

	uint32_t		m_wait_time;
	std::string		m_rest_file;
	config_t		m_curr_config;
	std::list<config_t> m_configs;
	HandleComFrame		m_hcf;
}; // class BoardController
} // namespace tt
} // sw
} // namespace senasic
