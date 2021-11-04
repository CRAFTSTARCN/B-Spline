#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>


class Throwable : public std::exception {

    protected:
    std::string msg;

    public:
    explicit Throwable(const std::string& compileMsg) {
        msg = compileMsg;
    }

    const char* what() const noexcept {
        return msg.c_str();
    }

    virtual std::string type() const {
        return "Throwable";
    }

    ~Throwable() {}
};

class ShaderCompileError : public Throwable {

    public:
    explicit ShaderCompileError(const std::string& compileMsg) : Throwable(compileMsg) {}

    std::string type() const {
        return "SHADER::COMPILE_ERR";
    }
};

class ShaderLinkError : public Throwable {

    public:
    explicit ShaderLinkError(const std::string& compileMsg) : Throwable(compileMsg) {}

    std::string type() const {
        return "SHADER::LINK_ERR";
    }
};

class IOException : public Throwable {
    public:
    explicit IOException(const std::string& compileMsg) : Throwable(compileMsg) {}

    std::string type() const {
        return "IOEXCEPTION";
    }
};

class InputException : public Throwable {
    public:
    explicit InputException(const std::string& compileMsg) : Throwable(compileMsg) {}

    std::string type() const {
        return "INPUTEXCEPTION";
    }
};

class OutOfBoundException : public Throwable {
    public:
    explicit OutOfBoundException(const std::string& compileMsg) : Throwable(compileMsg) {}

    std::string type() const {
        return "OUT_OF_BOUNDEXCEPTION";
    }
};

#endif