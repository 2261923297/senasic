#include "BoardController.h"

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



namespace senasic {
namespace sw {
namespace tt {
void BoardController::test_set()
{
	auto it = std::get<1>(m_curr_config).begin();
	auto sleep_key = std::get<1>(m_curr_config).end()--;
	int size = std::get<1>(m_curr_config).size();
	int i = 0;
	while (it != std::get<1>(m_curr_config).end())
	{
		set_wait_time(100);
		int nWrite = m_com->write(*it);
		m_com->waitForBytesWritten();
		m_com->flush();
		if (nWrite != it->size()) {
			fprintf(stderr
				, "com should write %d byte, but write %d byte"
				, it->size(), nWrite);
			return;
		}

		__logger_debug << str_val(i) << str_val(size) << "\n";
		if (i == 2) {
			__logger_debug << "getting...";
			set_wait_time(800);
		}
		std::cout << "current cmd: " << std::endl;
		print_qbytearray(*it);
		if (-1 == get()) {
			continue;
		}
		it++;
		i++;
	}
	return;
}

BoardController::BoardController
(const std::string& config_file, const std::string& board_com)
{
	std::string com_name;
	m_wait_time = 100;
	read_config_from_file(config_file);
	m_com = new QSerialPort;
	m_com->setPortName(QString(board_com.c_str()));
	__logger_debug << str_val(board_com) << "\n";
	m_com->setBaudRate(QSerialPort::Baud115200);
	m_com->open(QIODevice::ReadWrite);
}

BoardController::~BoardController() {
	//关闭串口
	m_com->clear();
	m_com->close();
	m_com->deleteLater();
	printf("BoardController delete\n");
}

int BoardController::get_cur_config()
{
	QSerialPort serial;
	if (m_configs.empty()) { return 0; }						// 板子配置结束
	auto qa = m_configs.front();

	set_board_config_name(std::get<0>(qa));
	m_ad->set_save_file_name(g_rest_root + std::get<0>(qa) + '\\');
	if (g_board_config_name.back() == '\r')
		g_board_config_name.pop_back();
//	std::cout << "g_board_config_name" << g_board_config_name << std::endl;
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
		m_com->waitForReadyRead(m_wait_time);
		tmp_qa = m_com->readAll();
		if (tmp_qa.size() <= 0) {
			break;
		}
		// __logger_debug << "\nwhat recv: " << std::endl;
		// print_qbytearray(tmp_qa);
		m_read_buf.push_back(tmp_qa);
	} while (1);
	const ComSt* cs = (const ComSt*)m_read_buf.constData();

	//__logger_debug << "nRecv = " << nRead << std::endl;
	//__logger_debug << "\nwhat recv: " << std::endl;
	//print_qbytearray(m_read_buf);
	m_com->flush();
	int nRead = m_read_buf.size();
	if (nRead > 0) {
		parse_frame(
			(uint8_t*)m_read_buf.constData()
			, m_read_buf.size());
		//__logger_debug << str_val(nWriteToFile) << std::endl;
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

		
		__logger_debug << str_val(i) << ", " <<  str_val(size) << "\n";
		if (i == 0) {
			__logger_debug << "sleeping...";
			set_wait_time(100);
			Sleep(1000);

			std::cout << "current cmd: ";
			print_qbytearray(*it);
			it++;
			i++;
			continue;
		}
		if (i == size - 1) {
			__logger_debug << "getting...\n";
			set_wait_time(800);
		}
		__logger_debug << "current cmd: ";
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
	for (int i = 0; i < len; i++) {
		printf("%2x", data_beg[i]);
	}
	printf("\n");
	return len;
}

int BoardController::parse_frame(const uint8_t* frame, size_t len) {
	const ComSt* cs = nullptr;
	size_t pos = 0;
	uint8_t frame_tmp[256] = { 0 };
	int frame_len = 0;
	while (pos < len) {


		cs = (const ComSt*)(frame + pos);
		frame_len = cs->len + sizeof(ComSt);
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

void BoardController::print_configs()
{
	for (auto it : m_configs) {

	}
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
		__logger_debug << str_val(i) << ", " << str_val(current_context) << std::endl;
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
	std::get<0>(rt) = tmp_buf.substr(lens_pos[0], lens_pos[1] - 1);

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


} // namespace tt
} // sw
} // namespace senasic