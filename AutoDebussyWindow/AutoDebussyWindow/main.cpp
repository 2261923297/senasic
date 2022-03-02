#include "AutoDebussyWindow.h"
#include "BoardController.h"
#include "tools.h"

#include <QtWidgets/QApplication>
#include <qbytearray.h>
#include "AutoDebussy.h"
#include "Framework.h"

using namespace senasic::sw::tt;

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

using namespace senasic::sw::tt;
void test_port() {
    
    std::string config_dir
        , N900_ip, CMW500_ip, Board_COM
        , N900_cf, CMW500_cf, Board_cf;
    config_dir = "D:\\Auto\\Debussy\\configs\\";
    getAddressFromFile(Board_COM
        , config_dir + "Board_COM.txt");
    Board_cf = config_dir + "Board_test_items.txt";
    BoardController bc(Board_cf, Board_COM);
    bc.get_cur_config();
    /*
    BoardController::config_t single_config;
    std::get<0>(single_config) = "test_config";
    auto& bytes = std::get<1>(single_config);
    // 01 10 03 01 f4 65
    QByteArray qa;
    const char cmd1[] = { 0x01, 0x10, 0x03, 0x01, 0xf4, 0x65 };
    const char cmd2[] = { 0x01, 0x10, 0x03, 0x02, 0xc4, 0x06 };
    const char cmd3[] = { 0x01, 0x10, 0x03, 0x03, 0xd4, 0x27 };
    bytes.push_back(QByteArray(cmd1));
    bytes.push_back(QByteArray(cmd2));
    bytes.push_back(QByteArray(cmd3));
    
    bc.set_cur_config(single_config);
    */
    bc.set();
    return;
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
    // test_port();
    func_excute_time(Auto, time_cost);
    //__logger_debug << str_val(time_cost) << std::endl;

    // w.show();
    destory_win_network();
    return a.exec();
}
