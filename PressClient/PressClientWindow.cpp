#include "PressClientWindow.h"

PressClientWindow::PressClientWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    resize(800, 600);
    // read window
    m_read_window = new ReadWindow;
    m_read_window->setParent(this);
    m_read_window->setGeometry(0, 0, 800, 350);

    // back press client
    m_press_client  = new PressClient("192.168.1.187", 8080);
    m_press_client->connect();
    m_press_client->set_cmd_set("set press");
    m_press_client->set_press("max");
    m_press_client->set_cmd_get("give me press");
    m_press_client->add_reader(this->get_read_window());

    // write window
    m_write_window = new WriteWindow(this);
    m_write_window->setGeometry(0, 370, 800, 230);
    m_write_window->set_client(m_press_client);

    // btn send cmd
    m_btn_send = new QPushButton(this);
    m_btn_send->setText("set press");
    m_btn_send->setGeometry(700, 560, 80, 30);

    bind_icon();
}

void PressClientWindow::bind_icon()
{
    connect(m_btn_send, &QPushButton::clicked
        , m_write_window, &WriteWindow::handle_write);
}
