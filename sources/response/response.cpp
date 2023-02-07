#include "response.hpp"

template <typename T>
std::string NumberToString( T Number )
{
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}

ResponseC::ResponseC() {}

ResponseC::~ResponseC() {}

int ResponseC::getContentLength() { return body.length(); }

void ResponseC::setStatusCode( std::string code ) { this->statusCode = code; }

void ResponseC::setBody( std::string body ) { this->body = body; }

void ResponseC::makeResponseBody( std::string body )
{
    std::string response;

    response.append( "HTTP/1.1 " );
    response.append( statusCode );
    response.append( "\r\n" );
    response.append( "Content-Type: text/html\r\n" );
    response.append( "Content-Length: " );
    response.append( NumberToString( getContentLength() ) );
    response.append( "\r\n\r\n" );
    response.append( body );
}