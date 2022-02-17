#pragma once
#include <QListWidget>
#include "PressClient.h"

class ReadWindow : public QListWidget, public PressReader
{
public:
    ReadWindow(QWidget* parent = nullptr);
    ~ReadWindow();
    virtual void read_press(std::string& press) override;

    void writeLine(const std::string& str) { this->addItem(str.c_str()); }
    
private:

};



