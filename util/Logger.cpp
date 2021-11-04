#include "Logger.h"

#include <fstream> 

Logger::Logger() : stream(std::cout) {}

Logger::Logger(std::ostream& binded_stream) : stream(binded_stream) {}

void Logger::log(const std::string& content) {
    stream<<content<<std::endl;
}

void Logger::log(const char* content) {
    stream<<content<<std::endl;
}

void Logger::log(const Throwable& t) {
    stream<<t.type()<<": "<<t.what()<<std::endl;
}

Logger Logger::DEBUG = Logger();

Logger Logger::LOG   = Logger();

Logger Logger::ERROR = Logger(std::cerr);