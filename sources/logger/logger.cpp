#include "logger.hpp"

Logger::Logger() : _stream( std::cout.rdbuf() ), _levelState( 0 ) {}

Logger::~Logger() {}

// log level 0 is DEBUG, 1 is INFO, 2 is WARNING, 3 is ERROR, 4 is NONE

int Logger::_translate( std::string level )
{
    std::string levels[] = { DEBUG, INFO, WARNING, ERROR };

    for ( int i = 0; i < MAX_ITER_NUMBER; i++ ) {
        if ( levels[i] == level )
            return i;
    }
    return 4;
}

void Logger::setLogLevel( std::string level )
{
    this->_levelState = this->_translate( level );
}

std::string handleColor( std::string &loglevel )
{
    if ( loglevel == DEBUG )
        return COLOR_DEBUG;
    else if ( loglevel == INFO )
        return COLOR_INFO;
    else if ( loglevel == WARNING )
        return COLOR_WARNING;
    else if ( loglevel == ERROR )
        return COLOR_ERROR;
    else
        return COLOR_RESET;
}

bool denyPrint( std::string &loglevel, int levelState )
{
    if ( levelState == 1 && ( loglevel == DEBUG ) )
        return true;
    else if ( levelState == 2 && ( loglevel == DEBUG || loglevel == INFO ) )
        return true;
    else if ( levelState == 3
              && ( loglevel == DEBUG || loglevel == INFO
                   || loglevel == WARNING ) )
        return true;
    else if ( levelState == 4
              && ( loglevel == DEBUG || loglevel == INFO || loglevel == WARNING
                   || loglevel == ERROR ) )
        return true;
    return false;
}

void Logger::_printFmt( std::string loglevel, std::string msg )
{
    if ( denyPrint( loglevel, _levelState ) )
        return;

    struct timeval time;
    gettimeofday( &time, 0 );
    std::time_t t   = time.tv_sec;
    std::tm    *now = std::localtime( &t );

    char buffer[80];
    strftime( buffer, 80, "%H:%M:%S", now );

    _stream << handleColor( loglevel ) << "[" << std::setfill( ' ' )
            << std::setw( 7 ) << loglevel << " " << buffer << "."
            << time.tv_usec << "] " << msg << COLOR_RESET << std::endl;
}

void Logger::debug( std::string msg ) { _printFmt( DEBUG, msg ); }

void Logger::info( std::string msg ) { _printFmt( INFO, msg ); }

void Logger::warning( std::string msg ) { _printFmt( WARNING, msg ); }

void Logger::error( std::string msg ) { _printFmt( ERROR, msg ); }

void Logger::none( std::string msg ) { ( void ) msg; }

void Logger::setStream( std::ostream &stream )
{
    _stream.rdbuf( stream.rdbuf() );
}

std::ostream &Logger::getStream( void ) { return _stream; }
