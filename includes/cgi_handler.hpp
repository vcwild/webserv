#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include "webserv.hpp"
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

class Cgi_handler {

private:
    Request cgi_request;
    std::map<std::string, std::string>
                env_vars; // environment variables for the CGI process
    std::string response_body; // new body to be sent to the client

public:
    Cgi_handler();
    Cgi_handler( Request cgi_request );
    void        create_env_vars();
    std::string get_response_body();
    ~Cgi_handler();
    char **
    create_env_vars_array( std::map<std::string, std::string> &env_vars );
    std::string exec_cgi( std::string cgi_script_path, char **env_vars_array );
};

#endif

// A CGI path is the file location of a Common Gateway Interface (CGI) script on
// a web server. It is the location where the server can find the script to
// execute it. An example of a CGI path could be
// "/usr/local/cgi-bin/my_script.cgi" or
// "C:\inetpub\wwwroot\cgi-bin\my_script.pl" depending on the operating system
// and web server configuration. The path must be the full path on the file
// system, not a URL.
