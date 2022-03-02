#pragma once
#include "Framework.h"
#include "Socket.h" 
#include "structure.h"

#include <memory>  
#include <vector> 
#include <list> 

#include <qserialport.h>
#include <QtSerialPort/QSerialPortInfo>

namespace senasic {
namespace sw {
namespace tt {


class ComFrameWorker {
public:
	ComFrameWorker();
	virtual ~ComFrameWorker();
	virtual int work(const uint8_t* data, size_t len) = 0;

	void set_func_code(size_t fc) { m_func_code = fc; }
	size_t get_func_code() const { return m_func_code; }
	void set_ad(AutoDebussy* ad) { m_ad = ad; }
protected:
	size_t m_func_code;
	AutoDebussy* m_ad;
};

class RSPComFrameWorker : public ComFrameWorker {
public:
	RSPComFrameWorker() { m_func_code = 07; }
	~RSPComFrameWorker() { }
	virtual int work(const uint8_t* data, size_t len) override;
private:

};

class DataComFrameWorker : public ComFrameWorker {
public:
	DataComFrameWorker();
	~DataComFrameWorker() { }
	virtual int work(const uint8_t* data, size_t len) override;
private:
	std::string m_file_name;
	FILE* m_p_file;
};

class HandleFrame {
public:

protected:

};

class HandleComFrame {
public:
	HandleComFrame();
	~HandleComFrame();
	void set_ad(AutoDebussy* ad);
	int handle_frame
	(const uint8_t* frame, size_t frame_len);
private:
	std::map<size_t, ComFrameWorker*> m_workers;
};
} // namespace tt
} // sw
} // namespace senasic