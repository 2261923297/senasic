#include "PressClient.h"
#include "Transer.h"

#include <chrono>
#include <time.h>
#include <thread>
#include <functional>

PressClient::PressClient(const std::string& addr = "192.168.1.187", uint16_t port)
{
	set_port(port);
	set_addr(addr);

	m_transer = new TcpTranser;
	HandlePressClientError* he = new HandlePressClientError;
	he->set_transer(m_transer);
	m_transer->set_handle_error(he);
}

PressClient::~PressClient()
{
	close();
}

void PressClient::connect()
{
	TcpPath tp;
	tp.set_addr(m_addr);
	tp.set_port(m_port); 
	m_transer->connect(tp);
}

void PressClient::reconnect()
{

}

void PressClient::close()
{
	m_transer->close();
}

void PressClient::set()
{
	std::string cmd_set = m_cmdSetPress + m_setPress;
	
	m_transer->send(cmd_set);
}

void PressClient::get()
{
	m_transer->send(m_cmdGetPress);
}

void PressClient::wait()
{
	m_transer->recv(m_getPress);
}

void PressClient::notice()
{
	for (auto reader : m_readers) {
		reader->read_press(m_getPress);
	}
}

