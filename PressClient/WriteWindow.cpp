#include "WriteWindow.h"
#include "PressClient.h"

WriteWindow::WriteWindow(QWidget* parent)
{
	setParent(parent);
	m_client = nullptr;
}

WriteWindow::~WriteWindow()
{

}

void WriteWindow::handle_write()
{
	PressClient* pc = (PressClient*)m_client;

	pc->set_press(this->text().toStdString());
	pc->set();
}

void WriteWindow::set_client(Client* client)
{
	m_client = client;
}
