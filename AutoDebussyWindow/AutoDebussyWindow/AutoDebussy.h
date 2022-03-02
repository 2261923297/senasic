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

static std::string g_fre_level;
static std::string g_board_config_name;

static void set_board_config_name(const std::string& name) { g_board_config_name = name; }
static void set_fre_level(const std::string& name) { g_fre_level = name; }






