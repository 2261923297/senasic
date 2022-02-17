#pragma once
#include <winsock2.h>
#include <string>
#include <stdint.h>

class HandleError {
public:
	HandleError() { }
	virtual ~HandleError() { }

public:
	virtual void handle() = 0;
private:

}; // class HandleError

class Path {
public:
	virtual ~Path() { }
}; // class Path


class TcpPath : public Path
{
public:
	TcpPath();
	~TcpPath() { if(nullptr != m_addr) delete m_addr; }

	void 
		set_addr(const std::string& addr) { *m_addr = addr; }
	void 
		set_port(const uint16_t port) { m_port = port; }


	const std::string& 
		get_addr() const { return *m_addr; }
	const uint16_t& 
		get_port() const { return  m_port; }
private:
	std::string* m_addr;
	uint16_t     m_port;
};

class Socket {
public:
	Socket() { }
	virtual ~Socket() { }

	virtual int send(const std::string& str) = 0;
	virtual int recv(std::string& str) = 0;

	
	virtual int connect(Path* addr) = 0;
	virtual int reconnect(uint32_t n_msec) = 0;
	virtual int bind(Path* addr) = 0;
	virtual int close() = 0;

	virtual void 
		set_path(const Path* path) = 0;
	virtual const Path* 
		get_path() const = 0;

protected:
}; // Socket

class TcpSocket : public Socket
{
public:
	TcpSocket();
	~TcpSocket();
	void init();

	virtual int send(const std::string& str) override;
	virtual int recv(std::string& str) override;
	
	virtual int connect(Path* addr) override;
	virtual int reconnect(uint32_t n_msec) override;
	virtual int bind(Path* addr)override ;

	virtual int close() override;
	

	void set_addr(const std::string& addr) 
		{ m_path->set_addr(addr); }
	void set_port(uint16_t port) 
		{ m_path->set_port(port); }

	virtual void set_path(const Path* path) override 
		{ *m_path = *(TcpPath*)path; }
	virtual const Path* get_path() const override
		{ return  (Path*)m_path; }

	void handle_error(int is_error, const char* msg, int line);

	
private:
	TcpPath* m_path;
	TcpPath* m_bind_path;

	SOCKET   m_sock;
	char m_buffer[1024] = { 0 };

	HandleError* m_handle_error;
}; // class TcpSocket

