#ifndef WEBSERV_H
# define WEBSERV_H

#include <string>
#include <vector> 
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <poll.h>
#include <fcntl.h>

struct Route {
  std::string location_dir;
  std::vector<std::string> allowed_methods;
  std::string root;
  int client_max_body_size;
  std::vector<std::string> http_redirection;
  std::vector<std::string> cgi; 
  std::string autoindex; //directory listing 
};

struct Config {
  int listen_port;
  std::string server_name;
  std::string autoindex;
  std::vector<std::string> index_files;
  std::vector<std::string> allowed_methods;
  std::vector<Route> routes;
  std::string root_dir;
  std::vector<std::string> error_pages;
};

int basic_server();

#endif