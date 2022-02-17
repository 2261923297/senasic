#include "LogPanel.h"
#include <QtWidgets/QApplication>
#include "PressClient.h"
#include "Transer.h"
#include <qthread.h>
#include <thread>

void thrd_get(PressClient* pc) {
    while (true)
    {
        QThread::msleep(1000);
        pc->get();
    }
}

void thrd_wait(PressClient* pc) {
    while (true)
    {
        pc->wait();
        pc->notice();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LogPanel w;

    PressClient pc("192.168.1.187");
    pc.connect();
    pc.set_cmd_set("set press");
    pc.set_press("max");
    pc.set_cmd_get("give me press");
    pc.add_reader(&w);
    std::thread t_get(thrd_get, &pc);
    std::thread t_wait(thrd_wait, &pc);

    
    w.writeLine("testsettt!");

    w.show();
    return a.exec();
}
