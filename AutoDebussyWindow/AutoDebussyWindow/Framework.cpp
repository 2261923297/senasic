#include "Framework.h"
#include "tools.h"
#include "Socket.h"
#include "AutoDebussy.h"
#include "BoardController.h"

using namespace senasic::sw::tt;


InstrumentController::InstrumentController() {
	m_sock.reset();
	m_config_file = "./inst_config.txt";
}

int InstrumentController::init(const std::string& ip, uint16_t port) {

	m_sock = senasic::system::tt::Socket::ptr
	(new senasic::system::tt::TcpSocket);

	int res = read_config_from_file(m_config_file);
	if (-1 == res) { return -1; }

	senasic::system::tt::TcpPath tp;
	senasic::system::tt::HandleSocketError* hse
		= new senasic::system::tt::HandleSocketError;					// 会在socket 中被delete
	tp.set_addr(ip);
	tp.set_port(port);

	m_sock->set_handle_error(hse);
	bool rt = m_sock->connect(&tp);
	if (false == rt) { return -1; }
	return 0;
}

AutoDebussy::AutoDebussy()
{
	printf("AutoDebussy create!\n");
	m_N900_ctl = nullptr;
	m_CMW500_ctl = nullptr;
	m_board_ctl = nullptr;
	printf("AutoDebussy end!\n");
}

AutoDebussy::~AutoDebussy()
{
	delete m_board_ctl;
	delete m_CMW500_ctl;
	delete m_N900_ctl;
	printf("AutoDebussy destory!\n");
}

int AutoDebussy::init()
{
	std::string config_dir
		, N900_ip, CMW500_ip, Board_COM
		, N900_cf, CMW500_cf, Board_cf;
	config_dir = "D:\\Auto\\Debussy\\configs\\";

	getAddressFromFile(N900_ip
		, config_dir + "N900_ip.txt");
	getAddressFromFile(CMW500_ip
		, config_dir + "CMW500_ip.txt");
	getAddressFromFile(Board_COM
		, config_dir + "Board_COM.txt");

	CMW500_cf = config_dir + "CMW500_test_items.txt";
	Board_cf = config_dir + "Board_test_items.txt";
	
	m_CMW500_ctl = new CMW500Controller(CMW500_cf, CMW500_ip, 5025);
	m_board_ctl = new BoardController(Board_cf, Board_COM);
	
	m_CMW500_ctl->set_ad(this);
	m_board_ctl->set_ad(this);
	return 0;
}

int AutoDebussy::work() {
	m_N900_ctl->set();
	int res = 0;
	while (1) {
		// set board
		res = m_board_ctl->set();
		if (res == -1) { return -1; }
		if (res == 0) { break; }
		while (1) {
			// set cmw500
			res = m_CMW500_ctl->set();
			if (res == -1) { return -1; }
			if (res == 0) { break; }

			// save result to file
			res = m_board_ctl->get();
			Sleep(1000);
			if (res == -1) { return -1; }
		}
	}
	return 0;
}

int AutoDebussy::tpl_friday()
{
	while (1) {
		int board_get_cur_config = 0;
		printf("set Board:\n");
		board_get_cur_config = m_board_ctl->get_cur_config();
		if (board_get_cur_config == -1) { return -1; }
		if (board_get_cur_config == 0) { break; }

		while (1) {
			int cmw_set = 0, board_set2, board_get;
			printf("set CMW500: \n");
			cmw_set = m_CMW500_ctl->set();
			if (cmw_set == -1) { 
				__logger_error << "cmw_500 \n";
				return -1; 
			}
			
			printf("set Board: \n");
			board_set2 = m_board_ctl->set();
			if (board_set2 == -1) { 
				__logger_error << "board_set \n";
				return -1; 
			}
			if (cmw_set == 0) {
				__logger_debug << "cmw_500 is end\n";
				break;
			}
		}
	}
	return 0;
}
