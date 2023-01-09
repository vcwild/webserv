/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 09:43:44 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/03 09:48:28 by mvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "webserv.hpp"


//Acabar o parser.
//Lidar com erros basicos do arquivo de configuração.
//Fazer a conexão do parser com um servidor simples.

//The first server for a host:port will be the default for this host:port (that means
//it will answer to all the requests that don’t belong to an other server).
//The routes configuration.

Parser::Parser()
{

}

Parser::Parser(char *file_name) : file_name(file_name)
{

}

Parser::~Parser()
{

}

int Parser::parse() {
    int             server_flag = 0;
    int             location_flag = 0;
    Config          current_server;
    Route           current_route;
    std::ifstream   file(this->file_name);
    std::string     line;

    if (!file.is_open()) 
    {
        std::cout << "Error opening configuration file: " << this->file_name << std::endl;
        return 1;
    }


    while (std::getline(file, line)) 
    {
        std::istringstream line_stream(line);
        std::string key;
        line_stream >> key;
        if (key.empty())
            continue;
        if (location_flag == 1)
        {
            if (key == "allow_methods")
            {
                std::string value;
                while (line_stream >> value)
                {
                    current_route.allowed_methods.push_back(value);
                }
            }
            else if (key == "root")
            {
                std::string value;
                line_stream >> value;
                if (!value.empty())
                    current_route.root = value;
            }
            else if (key == "autoindex")
            {
                std::string value;
                line_stream >> value;
                if (!value.empty())
                    current_route.autoindex = value;
            }
            else if (key == "client_max_body_size")
            {
                std::string value;
                line_stream >> value;
                if (!value.empty())
                    current_route.root = atoi(value.c_str());
            }
            else if (key == "cgi")
            {
                std::string value;
                while (line_stream >> value)
                {
                    current_route.cgi.push_back(value);
                }
            }
            if (line.find('}') != std::string::npos) 
            {
                location_flag = 0;
                current_server.routes.push_back(current_route);
            }
        }
        else
        {
        if (key == "server") 
        {
            if (server_flag == 0)
            {
                current_server = Config();
                server_flag = 1;
            }
            else
            {
                this->servers_conf.push_back(current_server);
                current_server = Config();
            }
        }
        else if (key == "location")
        {
            location_flag = 1;
            current_route = Route();
            std::string value;
            line_stream >> value;
            if (!value.empty())
                current_route.location_dir = value;
        }
        else if (key == "listen")
        {
        std::string value;
        line_stream >> value;
        if (!value.empty())
            current_server.listen_port = atoi(value.c_str());
        }
        else if (key == "server_name")
        {
        std::string value;
        line_stream >> value;
        if (!value.empty())
            current_server.server_name = value;
        }
        else if (key == "autoindex")
        {
        std::string value;
        line_stream >> value;
        if (!value.empty())
            current_server.autoindex = value;
        }
        else if (key == "index")
        {
        std::string value;
        while (line_stream >> value)
        {
            current_server.index_files.push_back(value);
        }
        }
        else if (key == "allowed_method")
        {
            std::string value;
            while (line_stream >> value)
            {
                current_server.allowed_methods.push_back(value);
            }
        }
        else if (key == "root") 
        {
        std::string value;
        line_stream >> value;
        if (!value.empty())
            current_server.root_dir = value;
        }
        else if (key == "error_page") 
        {
        std::string value;
        line_stream >> value;
        if (!value.empty())
            current_server.error_pages.push_back(value);
        }
        }
    }
    this->servers_conf.push_back(current_server);
    return (0);
}

std::vector<Config> Parser::getServersConf() const 
{
  return this->servers_conf;
}
