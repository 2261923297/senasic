#pragma once
 

#include "Framework.h"
#include "Socket.h" 
#include "structure.h"

#include <memory>  
#include <vector> 
#include <list> 

#include <qserialport.h>
#include <QtSerialPort/QSerialPortInfo>

#pragma comment(lib, "ws2_32.lib") 
 
using namespace tt::framework;

namespace senasic { 
namespace sw { 
namespace tt { 
	 
class HandleInstrumentError : public senasic::system::tt::HandleError{
public:
	virtual void set_args(void* args) 
		{ m_inst_ctl = (InstrumentController*)args; }

protected:
	virtual void handle() {  }
private:
	InstrumentController* m_inst_ctl;
};
 


class CMW500Controller : public InstrumentController {
public:
	using ptr = std::shared_ptr<InstrumentController>;
	using config_t = std::tuple<std::string, std::string>;
	
	CMW500Controller(const std::string& config_file
					, const std::string& ip
					, uint16_t port);
	virtual ~CMW500Controller() {
		printf("CMW500Controller delete\n");
	}

	virtual int set() override;
	virtual int get() override;
	virtual int read_config_from_file(const std::string& file) override;

private:
	std::vector<config_t>  m_configs;
	size_t m_config_cur;
};

class N900Controller : public InstrumentController{
public:
	typedef std::shared_ptr<N900Controller> ptr;
	N900Controller(const std::string& config_file
		, const std::string& ip
		, uint16_t port);
	virtual ~N900Controller() {
		printf("N900Controller delete\n");
	}

	virtual int set() override;
	virtual int get() override;
	virtual int read_config_from_file(const std::string& file) override;

private:
	uint32_t	m_fid;
	std::string m_save_to_dir;
	std::string m_save_to_file;
	std::list<std::tuple<std::string, std::string> > m_configs;

}; // class N900Controller


class ComFrameWorker {
public:
	ComFrameWorker();
	virtual ~ComFrameWorker();
	virtual int work(const uint8_t* data, size_t len) = 0;
	void set_func_code(size_t fc) { m_func_code = fc; }
	size_t get_func_code() const { return m_func_code; }

protected:
	size_t m_func_code;
};

class RSPComFrameWorker : public ComFrameWorker {
public:
	RSPComFrameWorker() { m_func_code = 07; }
	~RSPComFrameWorker() { }
	virtual int work(const uint8_t* data, size_t len) override;
private:

};

class DataComFrameWorker : public ComFrameWorker {
public:
	DataComFrameWorker() { m_func_code = 0x20; }
	~DataComFrameWorker() { }
	virtual int work(const uint8_t* data, size_t len) override;
private:
	std::string m_file_name;
	FILE* m_p_file;
};
class HandleFrame {
public:

protected:

};

class HandleComFrame {
public:
	HandleComFrame();
	~HandleComFrame();

	int handle_frame
	(const uint8_t* frame, size_t frame_len);
private:
	std::map<size_t, ComFrameWorker*> m_workers;
};

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

	int parse_frame(const uint8_t* frame, size_t len);

	virtual int read_config_from_file
		(const std::string& file) override;
	// virtual int get_cur_config() override;
private:
	config_t framing(const char* buf);
	
private:
	QSerialPort		*m_com;
	QByteArray		m_read_buf = { 0 };

	std::string		m_rest_file;
	config_t m_curr_config;
	std::list<config_t> m_configs;
	HandleComFrame		m_hcf;
}; // class BoardController




} // namespace tt
} // sw
} // namespace senasic


