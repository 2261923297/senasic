#include "AutoDebussy.h"
#include "tools.h"
#include "structure.h"

#include <vector>
#include <string>
#include <stdio.h>
#include <tuple>
#include <iostream>
#include <qdebug.h>
#include <qobject.h>


std::string g_fre_level = "";
std::string g_board_config_name = "";
std::string g_rest_root = "D:\\Auto\\Debussy\\results\\Board\\";

using namespace tt::framework;

namespace senasic {
namespace sw {
namespace tt {


CMW500Controller::CMW500Controller(const std::string& config_file
	, const std::string& ip
	, uint16_t port) : InstrumentController()
{
	set_config_file(config_file);
	int res = init(ip, port);
	m_config_cur = 0;
	if (res == -1) {
		exit(-1);
	}
}

// 1 success, 0 false
int CMW500Controller::set()
{
	if (m_configs.size() == 0) {
		return 0;
	}
	int rt = 1;
	std::tuple<std::string, std::string> item 
		= m_configs[m_config_cur % m_configs.size()];
	m_config_cur++;
	if (m_config_cur % m_configs.size() == 0) {
		rt = 0;
	}

	std::string fre = std::get<0>(item);
	std::string levle = std::get<1>(item);
	std::string cmd_frequence
		= "SOURce:GPRF:GENerator:RFSettings:FREQuency " 
			+ fre
			+ "\n";

	std::string cmd_level
		= "SOURce:GPRF:GENerator:RFSettings:LEVEl "
			+ levle
			+ "\n";

	if (levle[0] == '-') {
		levle[0] = 'n';
	}
	g_fre_level = fre + "_" + levle + ".txt";
	
	if (false == m_sock->send(cmd_frequence)) { return -1; }
	if (false == m_sock->send(cmd_level)) { return -1; }
	
	return rt;
}

int CMW500Controller::get()
{
	std::string rest;
	m_sock->recv(rest);
	fprintf(stdout, "CMW500 return: %s\n", rest.c_str());
	return 0;
}

int CMW500Controller::read_config_from_file(const std::string& file)
{
	FILE* pFile = fopen(file.c_str(), "rb");
	if (pFile == nullptr) {
		fprintf(stderr, "can^t open file %s\n", file.c_str());
		return -1;
	}
	fseek(pFile, SEEK_SET, 0);
	char buffer[256] = { 0 };
	std::string config;
	std::string frequence;
	std::string level;

	int sp = 0, end = 0;
	while (fscanf(pFile, "%s\n", buffer) > 0) {
		config = buffer;
		sp = config.find_first_of(',');
		end = config.size();
		if (sp == std::string::npos || end == std::string::npos) {
			fprintf(stderr, "config_file format error file: %s\n", file.c_str());
			return -1;
		}
		frequence = config.substr(0, sp);
		level = config.substr(sp + 1, end - sp - 1);
		m_configs.push_back(std::tuple<std::string, std::string>
			(frequence, level));
		memset(buffer, 0, 256);
	}
	return 0;
}

int RSPComFrameWorker::work(const uint8_t* data, size_t len)
{
	//__logger_debug << "\nget_rsp, len = " << len << ": ";
	for (int i = 0; i < len; i++) {
		printf("%2x, ", (unsigned char)data[i]);
	}
	printf("\n");
	return 0;
}
int nWriteToFile = 0;

int DataComFrameWorker::work(const uint8_t* data, size_t len)
{
	//__logger_debug << "get_data, len = " << len << ": ";
	DATA_PKG dp;
	uint16_t crc = 0;
	changeSeq(data + len - 2, &crc, sizeof(crc));
	changeSeq(data
		, &dp.pkg_num, sizeof(DATA_SIZE));
	changeSeq(data + sizeof(DATA_SIZE)
		, &dp.pkg_seq, sizeof(DATA_SIZE));
	//__logfmt_debug("all_pkg_num = %d, cur_pkg_seq = %d, crc = 0x%x", dp.pkg_num, dp.pkg_seq, crc);

	if (dp.pkg_seq == 1) {
		m_file_name = g_rest_root + g_board_config_name + "\\" + g_fre_level;
		m_p_file = fopen(m_file_name.c_str(), "ab+");
		if (m_p_file == nullptr) {
			__logger_error << "cant open file " << m_file_name
				<< ", desc: " << strerror(errno) << std::endl;
			return -1;
		}
	}
	nWriteToFile += fwrite(data + sizeof(DATA_PKG), 1, len - 6, m_p_file);
	if (dp.pkg_seq == dp.pkg_num) {
		if(m_p_file != nullptr)
			fclose(m_p_file);
		m_p_file = nullptr;
	}
	//__logger_debug << str_val(nWriteToFile) << std::endl;
	return 0;
}

BoardController::~BoardController() {
	//关闭串口
	m_com->clear();
	m_com->close();
	m_com->deleteLater();
	printf("BoardController delete\n");
}

BoardController::BoardController
(const std::string& config_file, const std::string& board_com)
{
	std::string com_name;
	read_config_from_file(config_file);
	m_com = new QSerialPort;
	m_com->setPortName(QString(board_com.c_str()));
	__logger_debug << str_val(board_com) << "\n";
	m_com->setBaudRate(QSerialPort::Baud115200);
	m_com->open(QIODevice::ReadWrite);
}


int BoardController::get_cur_config()
{
	QSerialPort serial;
	if (m_configs.empty()) { return 0; }						// 板子配置结束
	auto qa = m_configs.front();

	g_board_config_name = std::get<0>(qa);
	if (g_board_config_name.back() == '\r')
		g_board_config_name.pop_back();
	std::cout << "g_board_config_name" << g_board_config_name << std::endl;
	m_curr_config = qa;

	m_configs.pop_front();
	char mkdir_buf[1024] = { 0 };
	sprintf(mkdir_buf, "mkdir %s"
		, (g_rest_root + g_board_config_name).c_str());
	::system(mkdir_buf);
	return 1;													// 不能写配置
}
int BoardController::get()
{
	QByteArray tmp_qa;
	m_read_buf.clear();
	do {
		m_com->waitForReadyRead(300);
		tmp_qa = m_com->readAll();
		if (tmp_qa.size() <= 0) {
			break;
		}
		m_read_buf.push_back(tmp_qa);
	} while (1);
	const ComSt* cs = (const ComSt*)m_read_buf.constData();

	////__logger_debug << "\nwhat recv: ";
	//print_qbytearray(m_read_buf);

	m_com->flush();
	int nRead = m_read_buf.size();
	if (nRead > 0) {
//		print_qbytearray(m_read_buf);
//		printf("\n\n\n");
		//__logger_debug << "nRecv = " << nRead << std::endl;
		parse_frame(
			(uint8_t*)m_read_buf.constData()
			, m_read_buf.size());
		//__logger_debug << str_val(nWriteToFile) << std::endl;
//		printf("\n\n\n");
	}
	else {
		__logfmt_error("com %s cant read data\n", (char*)m_com->portName().data());
		return -1;															// 不能读
	}
	m_read_buf.clear();
	return 1;
}
// only get curr_config
int BoardController::set() {
	auto it = std::get<1>(m_curr_config).begin();
	auto sleep_key = std::get<1>(m_curr_config).end()--;
	int size = std::get<1>(m_curr_config).size();
	int i = 0;
	while (it != std::get<1>(m_curr_config).end())
	{
		int nWrite = m_com->write(*it);
		m_com->waitForBytesWritten();
		m_com->flush();
		if (nWrite != it->size()) {
			fprintf(stderr
				, "com should write %d byte, but write %d byte"
				, it->size(), nWrite);
			return -1;
		}
		__logger_debug << str_val(i) << str_val(size) << "\n";	
		if (i == 0) {
			__logger_debug << "sleeping...";
			Sleep(1000);
			/*
			std::cout << "current cmd: " << std::endl;
			print_qbytearray(*it);
			it++;
			i++;
			continue;
			*/
		}
		std::cout << "current cmd: " << std::endl;
		print_qbytearray(*it);
		if (-1 == get()) {
			continue;
		}
		it++;
		i++;
	}
	return 1;										
}

int BoardController::get_ok(uint8_t len, const uint8_t* data_beg) {
	//__logger_debug << "len = " << (int)len << ": ";
	for (int i = 0; i < len; i++) {
		printf("%2x ", data_beg[i]);
	}
	std::cout << "get rsp!" << std::endl;
	for (int i = 0; i < len; i ++ ) {
		printf("%2x", data_beg[i]);
	}
	printf("\n");
	return len;
}


int BoardController::get_data(uint8_t len, const uint8_t* data_beg) {
//	//__logger_debug << "len = " << (int)len << ": ";
//	for (int i = 0; i < len; i++) {
//		printf("%2x ", data_beg[i]);
//	}

	m_rest_file = g_rest_root + g_board_config_name + '\\' + g_fre_level + ".txt";
	FILE* pFile = fopen(m_rest_file.c_str(), "ab+");
	if (pFile == nullptr) {
		fprintf(stderr, "can^t open file %s\n", m_rest_file.c_str());
		return -1;
	}
	fwrite(data_beg + sizeof(DATA_PKG), 1, len, pFile);

	fwrite("\n", 1, 1, pFile);
	fflush(pFile);
	return m_read_buf.size();
}

HandleComFrame::HandleComFrame()
{
	RSPComFrameWorker* rsp_worker = new RSPComFrameWorker;
	DataComFrameWorker* data_worker = new DataComFrameWorker;
	m_workers.insert({ rsp_worker->get_func_code(), rsp_worker });
	m_workers.insert({ data_worker->get_func_code(), data_worker });
}

HandleComFrame::~HandleComFrame()
{
	for (auto it : m_workers) {
		free(it.second);
	}
}

int HandleComFrame::handle_frame(const uint8_t* frame, size_t frame_len)
{
	const ComSt* cs = (const ComSt*)frame;
	auto it = m_workers.find(cs->func_code);
	if (it == m_workers.end()) {
		return m_workers[7]->work(frame + sizeof(*cs), cs->len);
	}
	else {
		return it->second->work(frame + sizeof(*cs), cs->len);
	}
	return -1;
}

int BoardController::parse_frame(const uint8_t* frame, size_t len) {
	const ComSt* cs = nullptr;
	size_t pos = 0;
	uint8_t frame_tmp[256] = { 0 };
	int frame_len = 0;
	while (pos < len) {
		
		cs = (const ComSt*)(frame + pos);
		frame_len = cs->len + sizeof(*cs);
		memcpy(frame_tmp, cs, frame_len);
																// check
		m_hcf.handle_frame(frame_tmp, frame_len);				// handle	
/*
		//__logger_debug << "pos = " << (int)pos 
			<< ", cs->len = " << (int)cs->len
			<< ", sizeof(*cs) = " << sizeof(*cs)
			<< std::endl;
//*/
		pos = pos + (cs->len + sizeof(*cs));
		memset(frame_tmp, 0, frame_len);
	}
	return 0;
}



int BoardController::read_config_from_file(const std::string& file)
{
	FILE* pFile = fopen(file.c_str(), "rb");
	if (pFile == nullptr) {
		fprintf(stderr, "can^t open file %s\n", file.c_str());
		return -1;
	}

	fseek(pFile, SEEK_SET, 0);
	char buffer[10240] = { 0 };
	int nRead = 1;

	memset(buffer, 0, 10240);
	nRead = fread(buffer, 1, 10239, pFile);
	if (nRead <= 0) { return -1; }
	std::string config_content = buffer;
	std::vector<int> sp_pos;
	int pos = 0;
	while (pos != std::string::npos) {
		sp_pos.push_back(pos);
		pos = config_content.find_first_of('-', pos + 1);
	}

	int n_add = 0;
	//__logger_debug << str_val(sp_pos.size()) << std::endl;
	for (int i = 1; i < sp_pos.size(); i++) {
		if (i != 1) {
			n_add = 3;			// windows /r/n
		}
		std::string current_context = config_content.substr(
			sp_pos[i - 1] + n_add
			, sp_pos[i] - sp_pos[i - 1] - n_add);
		//__logger_debug << str_val(sp_pos[i]) << std::endl;
		__logger_debug << str_val(i) << ", " 	<< str_val(current_context) << std::endl;
		m_configs.push_back(framing(
			current_context.c_str()));
	}
	return 0;
}

BoardController::config_t BoardController::framing(const char* buf) {
	config_t rt;
	std::string tmp_buf = buf;
	std::vector<int> lens_pos;
	int pos = 0;
	for (int i = 0; pos != std::string::npos; i++) {
		lens_pos.push_back(pos);
		pos = tmp_buf.find_first_of('\n', pos + 1);
	}
	tmp_buf.push_back(tmp_buf.size());
	std::get<0>(rt) = tmp_buf.substr(lens_pos[0], lens_pos[1]);

	auto& frams = std::get<1>(rt);
	for (int i = 2; i < lens_pos.size(); i++) {
		std::string len_bufs = tmp_buf.substr(lens_pos[i - 1] + 1
			, lens_pos[i] - lens_pos[i - 1] - 2);  // windows /r/n
		std::vector<uint8_t> frame;					// have, no crc, no len
		for (int j = 0; j < len_bufs.size(); j++) {
			if (len_bufs[j] != ',') {
				frame.push_back(len_bufs[j]);
			}
		}

		uint8_t len = frame.size() / 2 + 1;
		__logfmt_debug("frame_line: 0x%x, ", len);
		std::vector<uint8_t>config_item;			// no ',', have len no crc
		config_item.push_back('0');
		config_item.push_back('1');
		config_item.push_back(frame[0]);
		config_item.push_back(frame[1]);
		uint8_t str_len[2] = { 0 };
		CharToHex(len, str_len);
		config_item.push_back(str_len[0]);
		config_item.push_back(str_len[1]);

		for (int j = 2; j < frame.size(); j++) {
			config_item.push_back(frame[j]);
		}

		std::vector<uint8_t> real_frame;			// to byte array, have crc
		StrHexToBytes(config_item, real_frame);
		uint16_t crc = Crc16(real_frame.data(), real_frame.size()
			, 0x1021, 0xffff);
		changeSeq(&crc, str_len, 2);
		real_frame.push_back(str_len[0]);	// crc
		real_frame.push_back(str_len[1]);	// crc

		int trf_len = len + 3;
//		printf("trf_len = %x\n", trf_len);

		uint8_t* tmp_real_frame = (uint8_t*)malloc(trf_len);
		memcpy(tmp_real_frame, real_frame.data(), trf_len);
		
		QByteArray tmp_qa((const char*)tmp_real_frame, trf_len);
		print_qbytearray(tmp_qa);
		frams.push_back(tmp_qa);
		free(tmp_real_frame);
	}
	std::cout << "\n\n";
	return rt;
}

N900Controller::N900Controller
(const std::string& config_file
	, const std::string& ip
	, uint16_t port) : InstrumentController()
{
	m_fid = 0;
	m_save_to_dir = "D:\\Auto\\Debussy\\results\\N900\\";
	m_save_to_file = std::string("Debussy_") + __TIME__;
	set_config_file(config_file);
	int res = init(ip, port);
	if (res == -1) {
		exit(-1);
	}
}

int N900Controller::read_config_from_file
(const std::string& file)
{

	return 0;
}
int N900Controller::set()
{
	m_sock->send("Hi N900, there are some settings for you!\n");
	return 0;
}

int N900Controller::get()
{
	std::string ctx;
	std::string file_name = m_save_to_dir
		+ m_save_to_file + std::to_string(m_fid);
	FILE* pFile = nullptr;
	pFile = fopen(file_name.c_str(), "ab+");
	if (pFile == nullptr) {
		fprintf(stderr, "can^t open file %s\n", file_name.c_str());
		return -1;
	}
	fseek(pFile, SEEK_END, 0);

	m_sock->send("Hi N900, give me your result!\n");
	size_t nWrite = 0;
	while (m_sock->recv(ctx)) {
		nWrite = fwrite(ctx.c_str(), 1, ctx.size(), pFile);
		if (nWrite <= 0) {
			fprintf(stderr, "fwrite <= 0\n");
			return -1;
		}
	}
	return 0;
}



ComFrameWorker::ComFrameWorker()
{
}

ComFrameWorker::~ComFrameWorker()
{
}





} // namespace tt
} // sw
} // namespace senasic

