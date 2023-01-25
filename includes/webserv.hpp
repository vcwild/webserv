#ifndef WEBSERV_H
#define WEBSERV_H

#include "parsers.hpp"
#include <map>
#include <sstream>
#include <string>
#include <vector>

/* Definitions */
#define LISTEN_PORT_KEY "listen"
#define SERVER_NAME_KEY "server_name"
#define CLIENT_MAX_BODY_SIZE_KEY "client_max_body_size"
#define INDEX_KEY "index"
#define ALLOWED_METHOD_KEY "allowed_method"
#define ERROR_PAGE_KEY "error_page"
#define AUTOINDEX_KEY "autoindex"
#define ROUTE_KEY "location"
#define ROOT_KEY "root"
#define CGI_KEY "cgi"
#define HTTP_REDIRECTION_KEY "http_redirection"
#define ALLOW_METODS_KEY "allow_methods"

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
    std::string user_agent; // Mozilla/5.0 (X11; Linux x86_64; rv:78.0)
    std::string authorization;
    std::string query;
    std::string cgi_path;
};

struct Response
{
    std::string version; // HTTP/1.1
    std::string status_code; // 200
    std::string status_message; // OK
    std::string content_type; // text/html
    std::string content_length; // 1234
    std::string body;
};

std::vector<Config> parseConfig( string filename );

#endif
