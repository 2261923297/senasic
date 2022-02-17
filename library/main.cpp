#include <thread>
#include <mutex>
#include <iostream>
#include <Windows.h>
#include "Mutex.h"

int thrd_mutex_count = 0;

std::mutex mtx;
tt::StdMutex stdMtx;

void count_add_10k() {
	tt::StdMutex::Locker lock(stdMtx);
	for (int i = 0; i < 100000; i++) {
#if 0
		if (mtx.try_lock()) {		// 900000		
			thrd_mutex_count++;
			mtx.unlock();
		}
#else
		thrd_mutex_count++;
#endif
	}
}
void test_std_mutex() {

}


int main() {
	std::thread ths[10];
	for (int i = 0; i < 10; i++) {
		ths[i] = std::thread(count_add_10k);
	}

	for (auto& it : ths) { it.join(); }
	Sleep(1000);

	std::cout << "thrd_mutex_count: " << thrd_mutex_count << std::endl;

}