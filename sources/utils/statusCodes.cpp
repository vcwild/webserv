#include "statusCodes.hpp"

StatusCode::StatusCode()
{
    _statusCode[200] = "200 OK";
    _statusCode[201] = "201 Created";
    _statusCode[202] = "202 Accepted";
    _statusCode[204] = "204 No Content";
    _statusCode[300] = "300 Multiple Choice";
    _statusCode[301] = "301 Moved Permanently";
    _statusCode[302] = "302 Found";
    _statusCode[400] = "400 Bad Request";
    _statusCode[401] = "401 Unauthorized";
    _statusCode[403] = "403 Forbidden";
    _statusCode[404] = "404 Not Found";
    _statusCode[405] = "405 Method Not Allowed";
    _statusCode[413] = "413 Request Entity Too Large";
    _statusCode[415] = "415 Unsupported Media Type";
    _statusCode[500] = "500 Internal Server Error";
    _statusCode[502] = "502 Bad Gateway";
    _statusCode[504] = "504 Gateway Timeout";
    _statusCode[505] = "505 HTTP Version Not Supported";
}

StatusCode::~StatusCode() {}

std::string StatusCode::getStatusCode( int code ) { return _statusCode[code]; }