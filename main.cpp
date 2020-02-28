#include <iostream>
#include <thread>
#include "logger.h"

static L::Logger Log;

int main(int argc, char** argv) {

    Log.open();


    Log << "Log opened without errors. received argc " << argc << Flush;
    Log << "Program successfully started" << Flush;

    std::this_thread::sleep_for(std::chrono::seconds(2));

    Log << "Error occured in execution" << FlushCrit;

    Log << "Finished program" << Flush;

    Log.close();

//	Log << 100 << "hello" << "world";

//    Log << Flush;

//	std::thread th([]() {

//		for (int i = 0; i < 100; ++i) {

//            Log << "in thread 1" << Flush;

//            std::this_thread::sleep_for(std::chrono::milliseconds(200));
//		}

//	});

//	std::thread det([]() {

//		for (int i=0;;++i) {

//			Log << "hi from thread 2";

//			if (i % 5 == 0)
//                Log << Flush;

//			std::this_thread::sleep_for(std::chrono::milliseconds(500));
//		}

//	});

//	if (det.joinable())
//		det.detach();

//	th.join();

	return 0;
}
