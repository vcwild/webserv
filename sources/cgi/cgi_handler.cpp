/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 13:00:11 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/18 17:16:21 by mvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi_handler.hpp"

Cgi_handler::Cgi_handler() {

}

Cgi_handler::Cgi_handler(Request cgi_request) : cgi_request(cgi_request) {

}

Cgi_handler::~Cgi_handler() {

}

void Cgi_handler::create_env_vars()
{
    
    this->env_vars["AUTH_TYPE"] = cgi_request.authorization;
    this->env_vars["REDIRECT_STATUS"] = "200"; 
	this->env_vars["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->env_vars["SCRIPT_NAME"] = config.getPath(); 
	this->env_vars["SCRIPT_FILENAME"] = config.getPath();
	this->env_vars["REQUEST_METHOD"] = cgi_request.getMethod();
	this->env_vars["CONTENT_LENGTH"] = cgi_request.body.c_str(); //Converter para char*
	this->env_vars["CONTENT_TYPE"] = cgi_request.content_type;
	this->env_vars["PATH_INFO"] = request.getPath();
	this->env_vars["PATH_TRANSLATED"] = request.getPath();
	this->env_vars["QUERY_STRING"] = cgi_request.query; 
	this->env_vars["REMOTEaddr"] = cgi_request.port.c_str(); 
	this->env_vars["REMOTE_IDENT"] = cgi_request.authorization;
	this->env_vars["REMOTE_USER"] = cgi_request.authorization;
	this->env_vars["REQUEST_URI"] = request.getPath() + request.getQuery();
    this->env_vars["SERVER_NAME"] = cgi_request.host;
    this->env_vars["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->env_vars["SERVER_SOFTWARE"] = "Weebserv/1.0";
}

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
};

char **Cgi_handler::create_env_vars_array()
{
    
}