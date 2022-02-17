#include "ReadWindow.h"

ReadWindow::ReadWindow(QWidget* parent)
{
	this->setParent(parent);
}

ReadWindow::~ReadWindow()
{

}

void ReadWindow::read_press(std::string& press)
{
	writeLine(press);
}
