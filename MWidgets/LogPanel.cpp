#include "LogPanel.h"



LogPanel::LogPanel(QWidget *parent)
    : QListWidget(parent)
{
    ui.setupUi(this);
    resize(800, 600);
}



LogPanel::~LogPanel()
{

}

void LogPanel::read_press(std::string& press)
{
    this->writeLine(press);
}
