#pragma once

#include <string>
#include <queue>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <mutex>

#pragma warning(disable : 4996) //localtime

namespace L {

	using std::string;
	using std::queue;
	using std::ofstream;
	using std::timed_mutex;
	using std::lock_guard;

	enum class LogType : char {
		Info,
		Warning,
		Critical
	};

	/* write data when we get this type */
	/* TODO add defines for flushers */
	template<LogType T = LogType::Info>
	struct LogFlush {

		string strType() {
			switch (T)
			{
			case LogType::Info:
				return "Info";
			case LogType::Warning:
				return "Warning";
			case LogType::Critical:
				return "Critical";
			default:
				return "";
			}
		}
	};

	using Info = LogFlush<>;
	using Warn = LogFlush<LogType::Warning>;
	using Crit = LogFlush<LogType::Critical>;

	struct Logger {

		/* create file right here */
		Logger();

		/* open requested file */
		Logger(const string& filename);

		/* close file on destruction */
		~Logger();

		/* open requested file */
		void open(const string& filename);

		/* just close file */
		void close();

		/* write to file and flush only when receive LogFlush */
		template<LogType T>
		Logger& operator<<(LogFlush<T> lf) {

			if (!m_mutex.try_lock_for(std::chrono::seconds(5))) {
				return *this;
			}

			lock_guard<std::timed_mutex> lg(m_mutex, std::adopt_lock);

			if (!m_file)
				return *this;

			/* in future need to make customizable time format */
			auto now = std::chrono::system_clock::now();
			auto now_time = std::chrono::system_clock::to_time_t(now);

			m_file << std::put_time(std::localtime(&now_time), "%d.%m.%Y %X");
			m_file << ' ';

			for (; !m_queue.empty() && m_file;) {

				m_file << '[' << lf.strType() << "] " << m_queue.front() << '\n';

				m_queue.pop();
			}

			m_file.flush();

			return *this;
		}

		/* try to convert value and safe it */
		/* TODO add more to_string overloads for standard library */
		/* TODO rework to_string so it will be more flexible */
		template<typename T>
		Logger& operator<<(T val) {

			if (!m_mutex.try_lock_for(std::chrono::seconds(5))) {
				return *this;
			}

			lock_guard<std::timed_mutex> lg(m_mutex, std::adopt_lock);

			m_queue.push(std::to_string(std::forward<T>(val)));

			return *this;
		}

		/* overload because to_string not accepting strings */
		template<>
		Logger& operator<<(const string& val) {

			if (!m_mutex.try_lock_for(std::chrono::seconds(5))) {
				return *this;
			}

			lock_guard<std::timed_mutex> lg(m_mutex, std::adopt_lock);

			m_queue.push(val);

			return *this;
		}

		/* overload to use string literals */
		template<>
		Logger& operator<<(const char* val) {

			if (!m_mutex.try_lock_for(std::chrono::seconds(5))) {
				return *this;
			}

			lock_guard<std::timed_mutex> lg(m_mutex, std::adopt_lock);

			m_queue.push(string(val));

			return *this;
		}


	private:
		ofstream m_file; //file used for logging
		timed_mutex m_mutex; //queue mutex
		queue<string> m_queue; //queue will be written to file on flush
	};

} //LOGGER namespace