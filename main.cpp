#include <iostream>
#include <thread>
#include "logger.h"

static L::Logger Log("asd.txt");

int main(int argc, char** argv) {

	Log << 100 << "hello" << "world";

	Log << L::Info();

	std::thread th([]() {

		for (int i = 0; i < 100; ++i) {

			Log << "in thread 1" << L::Warn();

			std::this_thread::sleep_for(std::chrono::milliseconds(400));
		}

	});

	std::thread det([]() {

		for (int i=0;;++i) {

			Log << "hi from thread 2";

			if (i % 5 == 0)
				Log << L::Crit();

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

	});

	if (det.joinable())
		det.detach();

	th.join();

	return 0;
}