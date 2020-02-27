#include "logger.h"

namespace L {

	Logger::Logger() {
		m_file.open("output.log", std::ios_base::trunc);
	}

	Logger::Logger(const string& filename) {
		m_file.open(filename, std::ios_base::app);
	}

	Logger::~Logger() {
		m_file.close();
	}

	void Logger::open(const string& filename) {
		m_file.open(filename, std::ios_base::app);
	}

	void Logger::close() {
		m_file.close();
	}

} //LOGGER namespace