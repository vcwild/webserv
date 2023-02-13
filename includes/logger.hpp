#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <sys/time.h>

#define MAX_ITER_NUMBER 4
#define DEBUG "DEBUG"
#define INFO "INFO"
#define WARNING "WARNING"
#define ERROR "ERROR"
#define NONE "NONE"

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"

#define COLOR_DEBUG PURPLE
#define COLOR_INFO GREEN
#define COLOR_WARNING YELLOW
#define COLOR_ERROR RED
#define COLOR_RESET "\033[0m"

enum LogLevel { LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR };

class Logger {

public:
    Logger();
    ~Logger();

    void _printFmt( std::string loglevel, std::string msg );

    void          setLogLevel( std::string level );
    std::ostream &getStream( void );
    void          setStream( std::ostream &stream );
    void          debug( std::string msg );
    void          info( std::string msg );
    void          warning( std::string msg );
    void          error( std::string msg );
    void          none( std::string msg );

private:
    std::ostream _stream;
    int          _levelState;

    int _translate( std::string level );
};

#endif
