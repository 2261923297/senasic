#include "Transer.h"
#include <string>
#include <inttypes.h>
#include <stdio.h>
#include <errno.h>

bool TcpTranser::_connect(Path& path)
{
	m_path = *(TcpPath*)&path;

	m_tcp_sock.connectToHost(m_path.get_addr().c_str()
		, m_path.get_port());
	m_tcp_sock.waitForConnected(3000);
	return true;
}

bool TcpTranser::_reconnect()
{
	m_tcp_sock.connectToHost(m_path.get_addr().c_str()
		, m_path.get_port());
	m_tcp_sock.waitForConnected(3000);
	return true;
}

bool TcpTranser::_close()
{
	m_tcp_sock.close();
	return true;
}

bool TcpTranser::_send(const std::string& str)
{
	int rt = m_tcp_sock.write(str.c_str());
	m_tcp_sock.flush();
	return rt != 0;
}

bool TcpTranser::_recv(std::string& str)
{
	m_tcp_sock.waitForReadyRead();
	str = m_tcp_sock.read(1024).constData();
	return str.size() == 0;
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

TcpPath::TcpPath() { 
	m_addr = new std::string; 
	m_port = 8080;
}

bool Transer::connect(Path& path)
{
	bool rt = _connect(path);
	std::string str_msg = 
		std::string(__FILE__) + ":" + __FUNCTION__":";
	handle_error(rt, str_msg.c_str(), __LINE__);
	return rt;
}

bool Transer::reconnect()
{
	bool rt = _reconnect();
	std::string str_msg =
		std::string(__FILE__) + ":" + __FUNCTION__":";
	handle_error(rt, str_msg.c_str(), __LINE__);
	return rt;
	return false;
}

bool Transer::close()
{
	bool rt = _close();
	std::string str_msg =
		std::string(__FILE__) + ":" + __FUNCTION__":";
	handle_error(rt, str_msg.c_str(), __LINE__);
	return rt;
}

bool Transer::send(const std::string& str)
{
	bool rt = _send(str);
	std::string str_msg =
		std::string(__FILE__) + ":" + __FUNCTION__":";
	handle_error(rt, str_msg.c_str(), __LINE__);
	return rt;
}

bool Transer::recv(std::string& str)
{
	bool rt = _recv(str);
	std::string str_msg =
		std::string(__FILE__) + ":" + __FUNCTION__":";
	handle_error(rt, str_msg.c_str(), __LINE__);
	return rt;
}
