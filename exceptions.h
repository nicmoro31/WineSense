#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class DatabaseException : public std::runtime_error{
public:
    explicit DatabaseException(const std::string& message) : std::runtime_error(message){} 
};

class AutoclaveException : public std::runtime_error{
public:
    explicit AutoclaveException(const std::string& message) : std::runtime_error(message){} 
};

class SensorException : public std::runtime_error{
public:
    explicit SensorException(const std::string& message) : std::runtime_error(message){} 
};

class FileopenException : public std::runtime_error{
public:
    explicit FileopenException(const std::string& message) : std::runtime_error(message){} 
};

class MainWindowException : public std::runtime_error{
public:
    explicit MainWindowException(const std::string& message) : std::runtime_error(message){} 
};

class ChartException : public std::runtime_error{
public:
    explicit ChartException(const std::string& message) : std::runtime_error(message){} 
};

#endif // EXCEPTIONS_H
