/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 12:56:36 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/18 13:38:32 by mvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include "webserv.hpp"

class Cgi_handler {

  private:
    Request cgi_request;
    std::string cgi_path; // path to the CGI executable - /usr/bin/python3 or path to php cgi. 
    pid_t cgi_pid; // process ID of the CGI process
    int exit_status; // exit status of the CGI process
    std::map<std::string, std::string> env_vars; // environment variables for the CGI process


  public:
    Cgi_handler();
    Cgi_handler(Request cgi_request);
    ~Cgi_handler();
};

#endif