/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 09:44:34 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/25 10:38:54 by mvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "./server/Server.hpp"
#include "./parser/Parser.hpp"
#include "./cgi/Cgi_handler.hpp"

void print_config(std::vector<Config> servers_confs) {
std::vector<Config>::iterator config = servers_confs.begin();
while (config != servers_confs.end())
{
std::cout << "------------------NEW SERVER------------------" << std::endl; 
  std::cout << "Listen port: " << config->listen_port << std::endl;
  std::cout << "Server name: " << config->server_name << std::endl;
  std::cout << "Autoindex: " << config->autoindex << std::endl;
  std::cout << "Index files: ";
  for (std::vector<std::string>::const_iterator it = config->index.begin(); it != config->index.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  std::cout << "Allowed methods: ";
  for (std::vector<std::string>::const_iterator it = config->allowed_method.begin(); it != config->allowed_method.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  std::cout << "Root directory: " << config->root_dir << std::endl;
  std::cout << "Error pages: ";
  for (std::vector<std::string>::const_iterator it = config->error_page.begin(); it != config->error_page.end(); ++it) {
    std::cout << *it << " ";
  }
   std::cout << std::endl;
  std::cout << "--------------Routes------";
  std::cout << std::endl;
  for (std::vector<Route>::iterator it = config->routes.begin(); it != config->routes.end(); ++it) {
    std::cout << it->location_dir << std::endl;

std::cout << "Routes Methods:" << std::endl;
  for (std::vector<std::string>::const_iterator sit = it->allowed_method.begin(); sit != it->allowed_method.end(); ++sit) {
    std::cout << *sit << " ";
  }
  std::cout << std::endl;
  }
  std::cout << std::endl;
  std::cout << "------------------------------------" << std::endl; 
    config++;
}

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