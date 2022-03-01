#include "AutoDebussyWindow.h"
#include "tools.h"

#include <QtWidgets/QApplication>
#include "AutoDebussy.h"
#include "Framework.h"

using namespace senasic::sw::tt;
using namespace tt::framework;


void test_define_log() {
//    __logger_error << "hello logger!" << std::endl;
    std::cout << LOG_PROFIX << std::endl;
    std::cout << LOG_FORMAT(DEBUG) << std::endl;	
    _LOGGER(std::cout, DEBUG, LOG_FORMAT) 
        << "hello define log!" << std::endl;
    int a = 10;
    __logger_debug << "hello __logger_debug" << std::endl;
    __logger_error << "hello __logger_error" << std::endl;
    __logfmt_debug("hello, __va_args__ a = %d", a);
    __logfmt_error("hello, __va_args__ a = %d", a);
    
    // LOGFMT(stdout, INFO, LOG_FORMAT, "hello, __va_args__ a = %d", a);
}

void Auto() {
    AutoDebussy ad;
    ad.init();
    ad.tpl_friday();
}

int main(int argc, char *argv[])
{
    init_win_network();
    QApplication a(argc, argv);
    AutoDebussyWindow w;
    // test_define_log();
    // test_port();
    // printf("\ncd\n");
    uint64_t time_cost = 0;
    func_excute_time(Auto, time_cost);
    __logger_debug << str_val(time_cost) << std::endl;

    // w.show();
    destory_win_network();
    return a.exec();
}
