#ifndef STATUS_CODE_HPP
#define STATUS_CODE_HPP

#include <map>
#include <string>

class StatusCode {
public:
    StatusCode();
    ~StatusCode();
    std::string getStatusCode( int code );

private:
    std::map<int, std::string> _statusCode;
};

#endif
