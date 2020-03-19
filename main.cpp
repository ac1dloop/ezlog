#include <iostream>
#include <thread>
#include "logger.hpp"

#define Log L::getInstance()

int main(int argc, char** argv) {

    Log.open();


    Log << "Log opened without errors. received argc " << argc << Info();
    Log << "Program successfully started" << Info();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    Log << "Error occured in execution" << Crit();

    Log << "Finished program" << Warn();

    Log.close();

	return 0;
}
