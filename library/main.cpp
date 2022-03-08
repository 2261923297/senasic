#define WIN32_LEAN_AND_MEAN

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <Windows.h>

#include "Thread.h"
#include "Mutex.h"
#include "Socket.h"
#include "File.h"
#include "Timer.h"

#include <future>
#include <utility>

#pragma comment(lib, "ws2_32.lib")
int thrd_mutex_count = 0;

std::mutex mtx;
tt::system::StdMutex stdMtx;

void count_add_10k() {
	tt::system::StdMutex::Locker lock(stdMtx);
	for (int i = 0; i < 100000; i++) {
		thrd_mutex_count++;
	}
}

void test_std_mutex() {
	
}

void test_std_thread() {
	std::thread ths[10];
	for (int i = 0; i < 10; i++) {
		ths[i] = std::thread(count_add_10k);
	}

	for (auto& it : ths) { it.join(); }
	Sleep(1000);

	std::cout << "thrd_mutex_count: " 
			<< thrd_mutex_count << std::endl;
}

/*
void test_thread_pool() {
	tt::system::ThreadPool pool(4);
	std::vector< std::future<int> > results;

	for (int i = 0; i < 8; ++i) {
		results.emplace_back(
			pool.enqueue([i] {
				std::cout << "hello " << i << std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::cout << "world " << i << std::endl;
				return i * i;
				})
		);
	}

	for (auto&& result : results)
		std::cout << result.get() << ' ';
	std::cout << std::endl;
}
*/
void test_std_move() {
	std::string str = "Hello";
	std::vector<std::string> v;
	//调用常规的拷贝构造函数，新建字符数组，拷贝数据
	v.push_back(str);
	std::cout << "After copy, str is \"" << str << "\"\n";
	//调用移动构造函数，掏空str，掏空后，最好不要使用str
	v.push_back(std::move(str));
	
	std::cout << "After move, str is \"" << str << "\"\n";
	std::cout << "The contents of the vector are \"" << v[0]
		<< "\", \"" << v[1] << "\"\n";
}

#define tt senasic::system::tt

void test_Soket() {
	init_win_network();
	tt::Socket* tcpS = new tt::TcpSocket;
	tt::TcpPath tp;
	tt::HandleSocketError* hse = new tt::HandleSocketError;					// 会在socket 中被delete
	tp.set_addr("192.168.1.187");
	tp.set_port(8080);

	tcpS->set_handle_error(hse);
	tcpS->connect(&tp);
	std::string str_recv;
	while (1) { 
		tcpS->send("hello!");
		tcpS->recv(str_recv);
		std::cout << str_recv << std::endl;
	}

	destory_win_network();
}


void IntToHex(uint32_t integer, uint8_t* to)
{
	uint8_t c;
	int i, j, digit;
	for (i = 7, j = 0; i >= 0; i--, j++)
	{
		digit = (integer >> (i * 4)) & 0xf;
		if (digit < 10) {
			c = digit + 0x30;
		} else {
			c = digit + 0x37;
		}
		to[7 - i] = c;
	}
}

void change_file_data(const std::string& from_file, const std::string& to_file) {
	tt::File from, to;
	int read_size = 4 * 4 * 1024;
	from.open(from_file, "rb+");
	to.open(to_file, "wb");
	uint32_t *buffer = (uint32_t*)malloc(4 * 4 * 1024);
	uint8_t *buffer_write = (uint8_t*)malloc(40 * 1024);
	from.read(buffer, read_size);
	read_size /= 4;
	for (int i = 0; i < read_size; i++) {
		IntToHex(buffer[i], buffer_write + i * 10);
		buffer_write[i * 10 + 8] = '\r';
		buffer_write[i * 10 + 9] = '\n';
	}
	to.write(buffer_write, 40 * 1024);
	to.flush();
	free(buffer);
	free(buffer_write);
}

void test_change_file() {
	std::string root_dir =
		"C:\\Users\\senasic\\Desktop\\No14_ch17_LNA_1f_TIA_0_CBPF_8_CODE_19_2420~2460\\";
//		"D:\\Auto\\Debussy\\results\\Board\\fortest\\";
	std::string to_dir = "C:\\Users\\senasic\\Desktop\\ch17_hex\\";
	tt::File f;
	std::vector<std::string> m_files;
	f.get_all_file_in_dir(m_files, root_dir);
	for (auto it : m_files) {
		std::cout << "file_name: " << it << std::endl;
		change_file_data(root_dir + it, 
			to_dir + it.substr(0) + "_copy.txt");
	}

}

void timer_func(void* args) {
	std::cout << "timer func start" << std::endl;
}

#undef tt
void test_timer() {
	tt::system::Timer t(0, clock() + 3000, timer_func);
	t.run();
}

bool g_is_ready = false;
std::condition_variable g_cv;
// std::mutex mtx;

void th_func(int id) {
	std::unique_lock<std::mutex> ul(mtx);
	while (!g_is_ready) {
		g_cv.wait(ul);
	}
	std::cout << "threads is: " << id << std::endl;
}

void create() {
	std::unique_lock<std::mutex> ul(mtx);
	g_is_ready = true;
	g_cv.notify_one();
}

void test_condition_variable() {
	std::thread ths[10];
	for (int i = 0; i < 10; i++) {
		ths[i] = std::thread(th_func, i);
//		ths[i].detach();
	}

	std::cout << "all thread is ready!" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	create();
	for (int i = 0; i < 10; i++) {
		ths[i].join();
	}
	
}

void test_std_feature() {

}

int i = 0;
void task_func() {
	printf("task_func_%d_beg!\n", i++);
	Sleep(1000);
	printf("task_func_end!\n");
}

void test_scheduler() {
	int thread_num = 20;
	tt::system::Scheduler scheduler(thread_num / 2);
	for (int i = 0; i < 9; i++) {
		scheduler.add_task({ task_func });
	}
	scheduler.init();
//	scheduler.stop();
}
int main() {
	
//	test_std_thread();
//	test_std_move();
//	test_thread_pool();
//	test_Soket();
//	test_change_file();
//	test_timer();
//	test_condition_variable();
//	test_std_feature();
	test_scheduler();
}