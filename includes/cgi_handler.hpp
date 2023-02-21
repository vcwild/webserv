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
    // Attributes
    Request     _cgi_request;
    std::string _response_body; // new body to be sent to the client
    char      **_env_vars_array;

    // Methods
    char **
    _create_env_vars_array( std::map<std::string, std::string> env_vars );
    std::map<std::string, std::string> _create_env_vars();

public:
    // Constructors and destructors
    Cgi_handler();
    Cgi_handler( Request _cgi_request );
    ~Cgi_handler();

    // Methods
    std::string get_response_body();
    void        run();
    std::string exec_cgi( std::string cgi_script_path );
};

#endif

// A CGI path is the file location of a Common Gateway Interface (CGI) script on
// a web server. It is the location where the server can find the script to
// execute it. An example of a CGI path could be
// "/usr/local/cgi-bin/my_script.cgi" or
// "C:\inetpub\wwwroot\cgi-bin\my_script.pl" depending on the operating system
// and web server configuration. The path must be the full path on the file
// system, not a URL.
