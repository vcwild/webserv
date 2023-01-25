/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 09:28:15 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/25 10:59:38 by mvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "webserv.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <cerrno>

class Server {

  private:
    std::vector<Config> servers_conf;
    std::vector<int> sockets;
    bool running;

  public:
    Server();
    Server(std::vector<Config> servers_conf);
    ~Server();
    int start();
    int create_sockets();
    int read_request_data(int socket ,int request_buf_size);
    int send_basic_response(int socketfd);
    int handle_request_data();
    void accept_connections();
    void close_sockets_fd();
    void stop();
};

#endif