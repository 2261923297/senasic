#pragma once
#include <string>
#include <stdint.h>

#include <QTcpSocket>
#include "Socket.h"

class Transer;

class HandlePressClientError : public HandleError {
public:
	virtual void handle() override;
	void set_transer(Transer* transer)
	{
		m_transer = transer;
	}
private:
	Transer* m_transer;
}; // class HandlePressClientError

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

	// rt = true, func success
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
	TcpTranser();
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
	Socket		 *m_sock;
	TcpPath		 m_path;
}; // class TcpTranser

