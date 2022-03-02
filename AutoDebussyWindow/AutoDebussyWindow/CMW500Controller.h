#pragma once

#include "Framework.h"
#include "Socket.h" 
#include "structure.h"
#include "AutoDebussy.h"
#include "Framework.h"

#include <memory>  
#include <vector> 
#include <list> 

#include <qserialport.h>
#include <QtSerialPort/QSerialPortInfo>

#pragma comment(lib, "ws2_32.lib") 

// using namespace framework;

namespace senasic {
namespace sw {
namespace tt {

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


} // namespace tt
} // sw
} // namespace senasic
