#pragma once

#include <QtWidgets/QWidget>
#include <QListWidget>
#include "ui_LogPanel.h"
#include "PressClient.h"

class LogPanel : public QListWidget , public PressReader
{
    Q_OBJECT
public:
    LogPanel(QWidget *parent = Q_NULLPTR);
    ~LogPanel();
    void writeLine(const std::string& str) { this->addItem(str.c_str()); }
    virtual void read_press(std::string& press) override;


private:
    Ui::LogPanelClass ui;
};
