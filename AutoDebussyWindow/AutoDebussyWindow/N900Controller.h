#pragma once

#include "Framework.h"
#include "Socket.h" 
#include "structure.h"
#include "HandleComFrame.h"
#include "CMW500Controller.h"

#include <memory>  
#include <vector> 
#include <list> 

#include <qserialport.h>
#include <QtSerialPort/QSerialPortInfo>

// using namespace tt::framework;
namespace senasic {
namespace sw {
namespace tt {

class N900Controller : public InstrumentController {
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


} // namespace tt
} // sw
} // namespace senasic
