/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 13:00:11 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/18 17:50:30 by mvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi_handler.hpp"

Cgi_handler::Cgi_handler() {

}

Cgi_handler::Cgi_handler(Request cgi_request) : cgi_request(cgi_request) 
{
    this->create_env_vars();
    char **env_vars = this->create_env_vars_array();
    for (int i = 0; env_vars[i]; i++) {
        std::cout << env_vars[i] << std::endl;
    }
}

Cgi_handler::~Cgi_handler() {

}

    /*The value of "SCRIPT_NAME" should be the URL path to the CGI script.
For example, if the script is located at "http://example.com/cgi-bin/myscript.php" then the value of "SCRIPT_NAME" would be "/cgi-bin/myscript.php".
The value of "SCRIPT_FILENAME" should be the absolute file system path to the CGI script.
For example, if the script is located at "/var/www/html/cgi-bin/myscript.php" then the value of "SCRIPT_FILENAME" would be "/var/www/html/cgi-bin/myscript.php".
*/

void Cgi_handler::create_env_vars()
{
    //https://github.com/cclaude42/webserv/blob/master/srcs/cgi/CgiHandler.cpp
    this->env_vars["AUTH_TYPE"] = cgi_request.authorization;
    this->env_vars["REDIRECT_STATUS"] = "200"; 
	this->env_vars["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->env_vars["SCRIPT_NAME"] = cgi_request.cgi_path; 
	this->env_vars["SCRIPT_FILENAME"] = cgi_request.cgi_path; 
	this->env_vars["REQUEST_METHOD"] = cgi_request.method;
	this->env_vars["CONTENT_LENGTH"] = cgi_request.body; //Converter para char*
	this->env_vars["CONTENT_TYPE"] = cgi_request.content_type;
	this->env_vars["PATH_INFO"] = cgi_request.cgi_path; 
	this->env_vars["PATH_TRANSLATED"] = cgi_request.cgi_path; 
	this->env_vars["QUERY_STRING"] = cgi_request.query; 
	this->env_vars["REMOTEaddr"] = cgi_request.port; 
	this->env_vars["REMOTE_IDENT"] = cgi_request.authorization;
	this->env_vars["REMOTE_USER"] = cgi_request.authorization;
	this->env_vars["REQUEST_URI"] = cgi_request.cgi_path + cgi_request.query;
    this->env_vars["SERVER_NAME"] = cgi_request.host;
    this->env_vars["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->env_vars["SERVER_SOFTWARE"] = "Weebserv/1.0";
}

char **Cgi_handler::create_env_vars_array(td::map<std::string, std::string>& env_vars)
{
    int size = env_vars.size();
    char** result = new char*[size + 1];
    std::map<std::string, std::string>::const_iterator it;
    int i = 0;
    for (it = env_vars.begin(); it != env_vars.end(); it++) {
        std::string env_var = it->first + "=" + it->second;
        result[i] = new char[env_var.length() + 1];
        std::strcpy(result[i], env_var.c_str());
        i++;
    }
    result[size] = 0;
    return result;
}
}