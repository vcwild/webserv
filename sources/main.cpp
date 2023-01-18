/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 09:44:34 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/18 17:46:17 by mvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "./server/Server.hpp"
#include "./parser/Parser.hpp"

void print_config(std::vector<Config> servers_confs) {
    std::vector<Config>::iterator config = servers_confs.begin();
while (config != servers_confs.end())
{
std::cout << "------------------NEW SERVER------------------" << std::endl; 
  std::cout << "Listen port: " << config->listen_port << std::endl;
  std::cout << "Server name: " << config->server_name << std::endl;
  std::cout << "Autoindex: " << config->autoindex << std::endl;
  std::cout << "Index files: ";
  for (std::vector<std::string>::const_iterator it = config->index_files.begin(); it != config->index_files.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  std::cout << "Allowed methods: ";
  for (std::vector<std::string>::const_iterator it = config->allowed_methods.begin(); it != config->allowed_methods.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  std::cout << "Root directory: " << config->root_dir << std::endl;
  std::cout << "Error pages: ";
  for (std::vector<std::string>::const_iterator it = config->error_pages.begin(); it != config->error_pages.end(); ++it) {
    std::cout << *it << " ";
  }
   std::cout << std::endl;
  std::cout << "--------------Routes------";
  std::cout << std::endl;
  for (std::vector<Route>::iterator it = config->routes.begin(); it != config->routes.end(); ++it) {
    std::cout << it->location_dir << std::endl;

std::cout << "Routes Methods:" << std::endl;
  for (std::vector<std::string>::const_iterator sit = it->allowed_methods.begin(); sit != it->allowed_methods.end(); ++sit) {
    std::cout << *sit << " ";
  }
  std::cout << std::endl;
  }
  std::cout << std::endl;
  std::cout << "------------------------------------" << std::endl; 
    config++;
}

}

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
    req.cgi_path = "/cgi-bin/test.py";
    return req;
}

int main(int argc, char** argv) 
{
    if (argc > 5)
        std::cout << "Error" << std::endl;
    Parser parser(argv[1]);
    if(!parser.parse())
        std::cout << "Conf file is ok" << std::endl;
    else
    {
        std::cout << "Conf file error" << std::endl;
        return (1);
    }
    print_config(parser.getServersConf());
    Server server(parser.getServersConf());
    return 0;
}