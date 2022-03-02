#include "HandleComFrame.h"
#include "AutoDebussy.h"

#include "tools.h"
#include "structure.h"

#include <stdio.h>


namespace senasic {
namespace sw {
namespace tt {
int RSPComFrameWorker::work(const uint8_t* data, size_t len)
{
	__logger_debug << "get_rsp, len = " << len << ": ";
	for (int i = 0; i < len; i++) {
		printf("%2x, ", (unsigned char)data[i]);
	}
	printf("\n");
	return 0;
}
int nWriteToFile = 0;

DataComFrameWorker::DataComFrameWorker() { 
	m_func_code = 0x20; 
	m_p_file = nullptr;
}

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
		m_file_name = m_ad->m_save_file_name + m_ad->m_file_name;
		__logger_debug << "save_to_file: " << m_file_name << std::endl;
		
		m_p_file = fopen(m_file_name.c_str(), "ab+");
		if (m_p_file == nullptr) {
			__logger_error << "cant open file " << m_file_name
				<< ", desc: " << strerror(errno) << std::endl;
			return -1;
		}
	}

	nWriteToFile += fwrite(data + sizeof(DATA_PKG), 1, len - 6, m_p_file);
	fflush(m_p_file);

	if (dp.pkg_seq == dp.pkg_num) {
		if (m_p_file != nullptr)
			fclose(m_p_file);
		m_p_file = nullptr;
	}
	//__logger_debug << str_val(nWriteToFile) << std::endl;
	return 0;
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

void HandleComFrame::set_ad(AutoDebussy* ad)
{
	for (auto it : m_workers) {
		it.second->set_ad(ad);
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

ComFrameWorker::ComFrameWorker()
{
	m_func_code = -1;
}

ComFrameWorker::~ComFrameWorker()
{
}


} // namespace tt
} // sw
} // namespace senasic
