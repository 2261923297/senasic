#pragma once
#include <string>
#include "Transer.h"

class Transer;

class HandlePressClientError : public HandleError {
public:
	virtual void handle() override { 
		m_transer->reconnect();
	}
	void set_transer(Transer* transer) 
		{ m_transer = transer; }
private:
	Transer* m_transer;
}; // class HandlePressClientError

class Client {
public:
	virtual void connect() = 0;
	virtual void reconnect() = 0;
	virtual void close() = 0;

	virtual void set() = 0;
	virtual void get() = 0;
	virtual void wait() = 0;
	virtual void notice() = 0;
};


class PressReader {
public:
	virtual void 
	read_press(std::string& press) = 0;
protected:

}; // PressReader

class PressClient : public Client
{
public:
	PressClient(const std::string& addr, uint16_t port = 8080);
	~PressClient();

	void connect() override;
	void reconnect() override;
	void close() override;

	void set() override;
	void get() override;
	void wait() override;
	void notice() override;

public:
	
	void  set_press(const std::string& str) { m_setPress = str; }

	void set_cmd_get(const std::string& cmd) { m_cmdGetPress = cmd;  }
	void set_cmd_set(const std::string& cmd) { m_cmdSetPress = cmd;  }
	void set_port(const uint16_t port) { m_port = port; }
	void set_addr(const std::string& addr) { m_addr = addr; }

	const std::string& get_addr() const { return m_addr; }
	const uint16_t& get_port() const { return m_port; }
	const std::string& get_press() const { return m_getPress; }

	void add_reader(PressReader* pr) 
	 { m_readers.push_back(pr); }

private:
	std::string m_cmdGetPress;
	std::string m_cmdSetPress;
	
	std::string m_setPress;
	std::string m_getPress;
	Transer*	m_transer;
	std::string m_addr;
	uint16_t    m_port;

	std::vector<PressReader*> m_readers;
}; // class PressClient

