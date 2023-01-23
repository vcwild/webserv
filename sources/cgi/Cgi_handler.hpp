/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 12:56:36 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/23 09:16:05 by mvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include "webserv.hpp"

class Cgi_handler {

  private:
    Request cgi_request;
    std::map<std::string, std::string> env_vars; // environment variables for the CGI process
    std::string cgi_path; // path to the CGI executable - /usr/bin/python3 or path to php cgi. 
    pid_t cgi_pid; // process ID of the CGI process
    int exit_status; // exit status of the CGI process
   


  public:
    Cgi_handler();
    Cgi_handler(Request cgi_request);
    void create_env_vars();
    char **create_env_vars_arra();
    ~Cgi_handler();
    char ** create_env_vars_array(std::map<std::string,std::string>& env_vars);
};

#endif

//A CGI path is the file location of a Common Gateway Interface (CGI) script on a web server. It is the location where the server can find the script to execute it. An example of a CGI path could be "/usr/local/cgi-bin/my_script.cgi" or "C:\inetpub\wwwroot\cgi-bin\my_script.pl" depending on the operating system and web server configuration. The path must be the full path on the file system, not a URL.



