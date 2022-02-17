#pragma once

#include <QtWidgets/QMainWindow>
#include <qpushbutton.h>

#include "ui_PressClientWindow.h"
#include "ReadWindow.h"
#include "WriteWindow.h"
#include "PressClient.h"

class PressClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    PressClientWindow(QWidget *parent = Q_NULLPTR);
    ReadWindow* get_read_window() 
        { return m_read_window; }

    PressClient* get_press_client() 
        { return m_press_client; }

    void bind_icon();
private:
    ReadWindow* m_read_window;
    WriteWindow* m_write_window;
    QPushButton* m_btn_send;
    PressClient* m_press_client;

    Ui::PressClientWindowClass ui;
};
