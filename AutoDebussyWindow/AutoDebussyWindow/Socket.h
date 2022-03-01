#pragma once
#include <winsock2.h>
#include <string>
#include <stdint.h>
#include <memory>

#include "HandleError.h"

void init_win_network();
void destory_win_network();

namespace senasic {
namespace system {
namespace tt {

using HandleError = senasic::tool::tt::HandleError;

class Path {
public:
	virtual ~Path() { }
}; // class Path


class TcpPath : public Path
{
public:
	TcpPath();
	~TcpPath() { if(nullptr != m_addr) delete m_addr; }

	void set_addr(const std::string& addr) { *m_addr = addr; }
	void set_port(const uint16_t port) { m_port = port; }

	const std::string& get_addr() const { return *m_addr; }
	const uint16_t& get_port() const { return  m_port; }
private:
	std::string* m_addr;
	uint16_t     m_port;
};

class Socket {
public:
	using ptr = std::shared_ptr<Socket>;
	Socket();
	virtual ~Socket();

	virtual bool socket();
	virtual bool send(const std::string& str);
	virtual bool recv(std::string& str);

	// close socekt _connect
	virtual bool connect(const Path* addr);
	virtual bool bind(const Path* addr);
	virtual bool reconnect(uint32_t n_msec = 3000);
	virtual bool close();

	virtual int _socket() = 0;
	virtual int _send(const std::string& str) = 0;
	virtual int _recv(std::string& str) = 0;

	virtual int _connect(const Path* addr) = 0;
	virtual int _bind(const Path* addr) = 0;
	virtual int _reconnect(uint32_t n_msec) = 0;
	virtual int _close() = 0;

	virtual void set_path(const Path* path) = 0;
	virtual const Path* get_path() const = 0;

	// 会delete上个传入的 he
	void set_handle_error(HandleError* he);
protected:
	HandleError* m_he;
}; // Socket

class HandleSocketError : public HandleError {
public:
	~HandleSocketError() { printf("HandleSocketError delete!\n"); }

	using ptr = std::shared_ptr<HandleSocketError>;

	virtual void set_args(void* args) override 
		{ m_sock = (Socket*)args; }
protected:
	void handle() override;

private:
	Socket* m_sock;

}; // class HandleSocketError


class TcpSocket : public Socket
{
public:
	TcpSocket();
	~TcpSocket();

	virtual int _socket() override;
	virtual int _send(const std::string& str) override;
	virtual int _recv(std::string& str) override;
	
	virtual int _connect(const Path* addr) override;
	virtual int _reconnect(uint32_t n_msec) override;
	virtual int _bind(const Path* addr)override ;

	virtual int _close() override;
	

	void set_addr(const std::string& addr) 
		{ m_path->set_addr(addr); }
	void set_port(uint16_t port) 
		{ m_path->set_port(port); }

	virtual void set_path(const Path* path) override;
	virtual const Path* get_path() const override;
	
private:
	TcpPath* m_path;
	TcpPath* m_bind_path;

	SOCKET   m_sock;
	char m_buffer[1024] = { 0 };

}; // class TcpSocket

} // namespace tt
} // namespace system
} // namespace senasic