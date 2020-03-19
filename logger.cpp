#include <logger.hpp>

namespace L {

    Logger::Logger() {
    }

    Logger::Logger(const string& filename):m_filename(filename) {
        m_file.open(filename, std::ios_base::trunc);
    }

    Logger::~Logger() {
        m_file.close();
    }

    void Logger::open(const string& filename) {
        if (m_file)
            m_file.close();

        m_filename=filename;

        m_file.open(filename, std::ios_base::app);
    }

    void Logger::create(const string &filename){
        if (m_file)
            m_file.close();

        m_filename=filename;

        m_file.open(filename, std::ios_base::trunc);
    }

    void Logger::logCout(bool b){
        m_logCout=b;
    }

    void Logger::close() {
        m_file.close();
    }

    void Logger::recreate(){
    }

    template<>
    Logger& Logger::operator<<(string val) {
        return this->operator<<<const string&>(const_cast<const string&>(val));
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

    template<>
    Logger& Logger::operator<<(char* val) {
        return this->operator<<<const char*>(const_cast<const char*>(val));
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

    Logger& getInstance(){
        static L::Logger inst;
        return inst;
    }

} //LOGGER namespace
