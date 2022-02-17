#pragma once
#include <QLineEdit>

class Client;

class WriteWindow : public QLineEdit
{
public:
	WriteWindow(QWidget* parent = nullptr);
	~WriteWindow();

	void handle_write();
	void set_client(Client* client);    
	
private:
	Client* m_client;
}; // class WriteWindow


