#include "Transer.h"
#include <string>
#include <inttypes.h>
#include <stdio.h>
#include <errno.h>
#include <qthread.h>


bool Transer::connect(Path& path)
{
	bool rt = _connect(path);
	std::string str_msg =
		std::string(__FILE__) + ":" + __FUNCTION__":";
	
	handle_error(rt == 0, str_msg.c_str(), __LINE__);
	return rt != -1;
}

bool Transer::reconnect()
{
	bool rt = _reconnect();
	std::string str_msg = 
		std::string(__FILE__) + ":" + __FUNCTION__":";
	handle_error(rt != -1, str_msg.c_str(), __LINE__);
	return rt == 0;
}

bool Transer::close()
{
	bool rt = _close();
	std::string str_msg =
		std::string(__FILE__) + ":" + __FUNCTION__":";
	handle_error(rt != -1, str_msg.c_str(), __LINE__);
	return rt;
}

bool Transer::send(const std::string& str)
{
	bool rt = _send(str);
	std::string str_msg =
		std::string(__FILE__) + ":" + __FUNCTION__":";
	handle_error(rt != -1, str_msg.c_str(), __LINE__);
	return rt;
}

bool Transer::recv(std::string& str)
{
	bool rt = _recv(str);
	std::string str_msg =
		std::string(__FILE__) + ":" + __FUNCTION__":";
	handle_error(rt != -1, str_msg.c_str(), __LINE__);
	return rt;
}

TcpTranser::TcpTranser()
{
	m_sock = new TcpSocket();
}

bool TcpTranser::_connect(Path& path)
{
	m_path = *(TcpPath*)&path;
	return m_sock->connect(&m_path);
}

bool TcpTranser::_reconnect()
{
	return m_sock->reconnect(3000);
}

bool TcpTranser::_close()
{
	return m_sock->close();
}

bool TcpTranser::_send(const std::string& str)
{
	int rt = m_sock->send(str.c_str());
	return rt == -1;
}

bool TcpTranser::_recv(std::string& str)
{
	return m_sock->recv(str);
}

void TcpTranser::handle_error(bool is_true, const char* msg, int line)
{
	if (!is_true) {
		fprintf(stdout, 
			"%s:%d> %s", 
			msg, line, strerror(errno));
		m_he->handle();
	}
}

void HandlePressClientError::handle() {
	m_transer->reconnect();
}
