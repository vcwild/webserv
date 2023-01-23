/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_main.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 09:44:34 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/23 10:25:15 by mvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "./server/Server.hpp"
#include "./parser/Parser.hpp"
#include "./cgi/Cgi_handler.hpp"


Request createSampleRequest() {
    Request req;
    req.method = "GET";
    req.host = "www.example.com";
    req.port = "80";
    req.uri = "/index.html";
    req.version = "HTTP/1.1";
    req.body = "";
    req.content_type = "text/html";
    req.user_agent = "Mozilla/5.0 (X11; Linux x86_64; rv:78.0)";
    req.authorization = "";
    req.query = "name=value";
    req.cgi_path = "./cgi-bin/basic.py";
    return req;
}

int main() 
{
    Request req;
    req = createSampleRequest();
    Cgi_handler cgi(req);
    return 0;
}