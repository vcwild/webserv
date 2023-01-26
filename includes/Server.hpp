#ifndef SERVER_HPP
#define SERVER_HPP

#include "webserv.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define REQUEST_SIZE 1024

class Server {

private:
    std::vector<Config> servers_conf;
    std::vector<int>    sockets;
    std::map<int, std::string>	requests;
    bool                running;

public:
    Server();
    Server( std::vector<Config> servers_conf );
    Server(const Server & src);
    //Server & operator=(const Server & src);
    ~Server();
    int  start();
    int  create_sockets();
    int  read_request_data( int socket , int request_size);
    int  send_basic_response( int socketfd );
    int  handle_request_data();
    void accept_connections();
    void close_sockets_fd();
    void stop();
};

#endif
