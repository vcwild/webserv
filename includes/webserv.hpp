#ifndef WEBSERV_H
#define WEBSERV_H

#include <arpa/inet.h>
#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <ostream>
#include <poll.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <unistd.h>
#include <sys/wait.h>

struct Route
{
    std::vector<std::string> allowed_method;
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
    std::string              client_max_body_size;
    std::vector<std::string> index;
    std::vector<std::string> allowed_method;
    std::vector<std::string> error_page;
    std::string              autoindex;
    std::vector<Route>       routes;
    std::string              root_dir;
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

#endif
