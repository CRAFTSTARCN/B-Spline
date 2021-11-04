#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include "Exception.hpp"
#include "typeindex"

class Logger {
    private:
    
    std::ostream& stream;

    public:

    Logger();

    Logger(std::ostream& binded_stream);

    static Logger DEBUG;
    static Logger LOG;
    static Logger ERROR;

    void log(const std::string& content);
    void log(const char* content);
    void log(const Throwable& t);

    template<typename T>
    void log(const T& t) {
        stream<<"< "<<typeid(T).name()<<" at: "<<&t<<" >"<<std::endl;
    }
};

#endif