#include "PressClientWindow.h"
#include <QtWidgets/QApplication>
#include "Socket.h"

#include <thread>
#include <string>
#include <qthread.h>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")



void test_tcpSocket() {


}

void thrd_get(PressClient* pc) {
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsdata;
    if (WSAStartup(sockVersion, &wsdata) != 0)
    {
        return;
    }
    QThread::msleep(1000);
    while (true)
    {
        QThread::msleep(1000);
        pc->get();
    }
    WSACleanup();
}

void thrd_wait(PressClient* pc) {
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsdata;
    if (WSAStartup(sockVersion, &wsdata) != 0)
    {
        return;
    }
    QThread::msleep(1000);
    while (true)
    {
        pc->wait();
        pc->notice();
    }
    WSACleanup();
}

int main(int argc, char *argv[])
{
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsdata;
    if (WSAStartup(sockVersion, &wsdata) != 0)
    {
        return 1;
    }

    QApplication a(argc, argv);
    PressClientWindow w;

    std::thread t_get(thrd_get, w.get_press_client());
    std::thread t_wait(thrd_wait, w.get_press_client());

    w.show();
    WSACleanup();
    return a.exec();
}
