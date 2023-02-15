#ifndef WEBSERV_H
#define WEBSERV_H

#include "logger.hpp"
#include "parsers.hpp"
#include "utils.hpp"
#include <map>
#include <sstream>
#include <string>
#include <vector>

/* Global variables */
static Logger logger;

/* Structs */
struct Route
{
    std::vector<std::string> allow_methods;
    std::string              root;
    std::string              index;
    std::vector<std::string> cgi;
    std::string              location_dir;
    int                      client_max_body_size;
    std::vector<std::string> http_redirection;
    std::string              autoindex; // directory listing
};

struct Config
{
    int                      listen_port;
    std::string              server_name;
    int                      client_max_body_size;
    std::vector<std::string> index;
    std::vector<std::string> allowed_method;
    std::vector<std::string> error_page;
    std::string              autoindex;
    std::vector<Route>       routes;
    std::string              root_dir;

    Config();

    Config( std::vector<std::string>::iterator &it,
            std::vector<std::string>::iterator &end );

    void setRouteAttributes( Route &route, std::string &line );

    void setConfig( std::string &line );
};

struct Request
{
    std::string method; // GET
    std::string host; // hostname
    std::string port; // port
    std::string uri; // /index.html
    std::string version; // HTTP/1.1
    std::string body; // request body
    std::string content_type; // text/html
    std::string content_length;
    std::string user_agent; // Mozilla/5.0 (X11; Linux x86_64; rv:78.0)
    std::string authorization;
    std::string accept;
    std::string query;
    std::string cgi_path;

    Request();

    Request( const char *buf );

    void setMethod( std::string &line );
    void setHost( std::string &line );
    void setUserAgent( std::string &line );
    void setAccept( std::string &line );
    void setConcentLength( std::string &line );
    void setContentType( std::string &line );
    void setBody( const char *buf );
};

struct Response
{
    std::string version; // HTTP/1.1
    std::string status_code; // 200
    std::string status_message; // OK
    std::string content_type; // text/html
    std::string content_length; // 1234
    std::string date; // Tue, 15 Nov 1994 08:12:31 GMT
    std::string keep_alive; // timeout=5, max=100
    std::string server; // Apache/2.4.1 (Unix)
    std::string connection; // Keep-Alive
    std::string accept_ranges; // bytes
    std::string body;

    Response();

    Response( char *buf );

    void setFirstLine( std::string &line );
    void setBody( char *buf );
};

std::vector<Config> parseConfig( string filename );

#endif
