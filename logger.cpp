#include "logger.h"

namespace L {

	Logger::Logger() {
	}

	Logger::Logger(const string& filename) {
        m_file.open(filename, std::ios_base::trunc);
	}

	Logger::~Logger() {
		m_file.close();
	}

	void Logger::open(const string& filename) {
        if (m_file)
            m_file.close();

		m_file.open(filename, std::ios_base::app);
	}

	void Logger::close() {
		m_file.close();
    }

    /* overload because to_string not accepting strings */
    template<>
    Logger& Logger::operator<<(const string& val) {

        if (!m_mutex.try_lock_for(std::chrono::seconds(5))) {
            return *this;
        }

        lock_guard<std::timed_mutex> lg(m_mutex, std::adopt_lock);

        m_queue.push(val);

        return *this;
    }

    /* overload to use string literals */
    template<>
    Logger& Logger::operator<<(const char* val) {

        if (!m_mutex.try_lock_for(std::chrono::seconds(5))) {
            return *this;
        }

        lock_guard<std::timed_mutex> lg(m_mutex, std::adopt_lock);

        m_queue.push(string(val));

        return *this;
    }

    template<>
    Logger& Logger::operator<<(char c){

        if (!m_mutex.try_lock_for(std::chrono::seconds(5))) {
            return *this;
        }

        lock_guard<std::timed_mutex> lg(m_mutex, std::adopt_lock);

        m_queue.push(string(1, c));

        return *this;
    }

} //LOGGER namespace
