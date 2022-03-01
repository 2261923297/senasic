#pragma once
 

#include "Framework.h"
#include "Socket.h" 
#include "structure.h"
#include "HandleComFrame.h"
#include "CMW500Controller.h"
#include "N900Controller.h"

#include <memory>  
#include <vector> 
#include <list> 

#include <qserialport.h>
#include <QtSerialPort/QSerialPortInfo>

#pragma comment(lib, "ws2_32.lib") 
 
static std::string g_fre_level = "";
static std::string g_board_config_name = "";
static std::string g_rest_root = "D:\\Auto\\Debussy\\results\\Board\\";



using namespace tt::framework;

namespace senasic { 
namespace sw { 
namespace tt { 
	 
class HandleInstrumentError : 
	public senasic::system::tt::HandleError{
public:
	virtual void set_args(void* args) 
		{ m_inst_ctl = (InstrumentController*)args; }

protected:
	virtual void handle() {  }
private:
	InstrumentController* m_inst_ctl;
};

} // namespace tt
} // sw
} // namespace senasic


