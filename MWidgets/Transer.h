#pragma once
#include <string>
#include <stdint.h>

#include <QTcpSocket>

class Path {

};

class HandleError {
public:
	HandleError() { }
	~HandleError() { }

public:
	virtual void handle() = 0;
private:

}; // class HandleError



class TcpPath : public Path
{
public:
	TcpPath();
	~TcpPath() { m_port = 8080; }
	void set_addr(const std::string& addr) { *m_addr = addr; }
	void set_port(const uint16_t port) { m_port = port; }

	const std::string& get_addr() const { return *m_addr; }
	const uint16_t& get_port() const { return m_port; }
private:
	std::string* m_addr;
	uint16_t     m_port;
};


class Transer
{
public:
	virtual ~Transer() { delete m_he; }
	virtual bool _connect(Path& path) = 0;
	virtual bool _close() = 0;
	virtual bool _send(const std::string& str) = 0;
	virtual bool _recv(std::string& str) = 0;
	virtual bool _reconnect() = 0;

	
	virtual void set_handle_error
	(HandleError* he) {
		m_he = he;
	};

	bool connect(Path& path);
	bool reconnect();
	bool close();
	bool send(const std::string& str);
	bool recv(std::string& str);

	virtual void handle_error(bool is_true, const char* msg, int line) = 0;
protected:
	HandleError* m_he;
}; // class Transer



class TcpTranser : public Transer{
public:
	~TcpTranser() {  }
	virtual bool _connect(Path& path) override;
	virtual bool _reconnect() override;
	virtual bool _close() override;
	virtual bool _send(const std::string& str) override;
	virtual bool _recv(std::string& str) override;
	virtual void handle_error(bool is_true, const char* msg, int line) override;
	
public:

private: 
	char buffer[1024] = { 0 };
	QTcpSocket	 m_tcp_sock;
	TcpPath		 m_path;
}; // class TcpTranser

