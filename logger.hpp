#pragma once

#include <string>
#include <queue>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <mutex>
#include <vector>
#include <iostream>

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

        void logCout(bool b);

        /* open requested file */
        void open(const string& filename="output.log");

        void create(const string& filename="output.log");

        /* just close file */
        void close();

        /* write to file and flush only when receive LogFlush */
        template<LogType T>
        Logger& operator<<(LogFlush<T> lf) {

            if (!m_mutex.try_lock_for(std::chrono::seconds(5))) {
                return *this;
            }

            lock_guard<std::timed_mutex> lg(m_mutex, std::adopt_lock);

            std::stringstream ss;

            if (!m_file)
                return *this;

            /* in future need to make customizable time format */
            auto now = std::chrono::system_clock::now();
            auto now_time = std::chrono::system_clock::to_time_t(now);

            ss << std::put_time(std::localtime(&now_time), "%d.%m.%Y %X");
            ss << ' ' << '[' << lf.strType() << "] ";

            for (; !m_queue.empty() && m_file;) {

                ss << m_queue.front() << ' ';

                m_queue.pop();
            }

            ss << '\n';

            m_file << ss.str();

            if (m_logCout)
                std::cout << ss.str() << std::endl;

            m_file.flush();

            ++m_index;
            if (m_index >= 1000){
                m_file.close();

                std::rename(m_filename.c_str(), std::string(m_filename + '.' + std::to_string(m_maxLogs)).c_str());
                ++m_maxLogs;

                m_file.open(m_filename, std::ios_base::trunc);
            }

            return *this;
        }

        template<typename T>
        Logger& operator<<(T val) {

            if (!m_mutex.try_lock_for(std::chrono::seconds(5))) {
                return *this;
            }

            lock_guard<std::timed_mutex> lg(m_mutex, std::adopt_lock);

            m_queue.push(std::to_string(std::forward<T>(val)));

            return *this;
        }

    private:

        /* creates new file with next index */
        void recreate();

        std::string m_filename;
        ofstream m_file; //file used for logging
        timed_mutex m_mutex; //queue mutex
        queue<string> m_queue; //queue will be written to file on flush
        bool m_logCout=false; //also log to cout
        unsigned m_index=0;
        unsigned m_maxLogs=0;
    };

    template<> Logger& Logger::operator<<(const char* val);
    template<> Logger& Logger::operator<<(char* val);
    template<> Logger& Logger::operator<<(const string& val);
    template<> Logger& Logger::operator<<(string val);
    template<> Logger& Logger::operator<<(char c);

    Logger& getInstance();

} //LOGGER namespace

#define FlushInfo L::Info()
#define FlushCrit L::Crit()
#define FlushWarn L::Warn()

using L::Info;
using L::Crit;
using L::Warn;

#define Log L::getInstance()
