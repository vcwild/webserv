/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:16:53 by mvieira-          #+#    #+#             */
/*   Updated: 2023/01/26 09:44:52 by mvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static void signal_handler( int sign )
{
    std::cout << "Signal received: " << sign << std::endl;
}

Server::Server() : running( true )
{
   
}

Server::Server( std::vector<Config> servers_conf ) : running( true )
{
    this->servers_conf = servers_conf;
    this->start();
}

Server::~Server()
{
    
}

int Server::start()
{
    this->create_sockets();
    this->accept_connections();
    return ( 0 );
}

int Server::create_sockets()
{
    std::vector<Config>::iterator it;
    for ( it = this->servers_conf.begin(); it != this->servers_conf.end();
          ++it ) {
        const Config &server_conf = *it;

        // create a new socket
        int sockfd = socket( AF_INET, SOCK_STREAM, 0 );
        if ( sockfd < 0 ) {
            std::cerr << "Error creating socket" << std::endl;
            return 1;
        }

        /*A non-blocking file descriptor is a file descriptor that allows the
        process to continue executing while waiting for an I/O operation to
        complete. This is in contrast to a blocking file descriptor, which
        causes the process to block (stop execution) until the I/O operation is
        completed. Non-blocking file descriptors are useful in situations where
        the process needs to perform multiple tasks concurrently, as they allow
        the process to perform other tasks while waiting for an I/O operation to
        complete. They are often used in event-driven programming and can be set
        using the fcntl function with the O_NONBLOCK flag.*/

        if ( fcntl( sockfd, F_SETFL, O_NONBLOCK ) < 0 ) {
            // there was an error setting the flags
            std::cerr << "Error setting flags for socket" << std::endl;
            return 1;
        }
        // bind the socket to the port specified in the configuration file
        struct sockaddr_in serv_addr;
        serv_addr.sin_family      = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port        = htons( server_conf.listen_port );

        if ( bind(
                 sockfd, ( struct sockaddr * ) &serv_addr, sizeof( serv_addr ) )
             < 0 ) {
            std::cerr << "Error binding socket to port "
                      << server_conf.listen_port << std::endl;
            return 1;
        }

        if ( listen( sockfd, SOMAXCONN ) < 0 ) {
            std::cerr << "Error setting socket to listen" << std::endl;
            return 1;
        }

        // add the socket file descriptor to the vector
        this->sockets.push_back( sockfd );
    }
    return 0;
}


void Server::accept_connections()
{
    int           nfds = this->sockets.size();
    struct pollfd fds[nfds];

    signal( SIGINT, signal_handler );

    for ( int i = 0; i < nfds; i++ ) {
        fds[i].fd     = this->sockets[i];
        fds[i].events = POLLIN;
    }
    while ( this->running ) {
        int ret = poll( fds, nfds, -1 );
        if ( ret < 0 ) {
            // there was an error with the poll function
            std::cerr << "Error with poll function" << std::endl;
            return;
        } else if ( ret == 0 ) {
            // the poll function timed out
            return;
        } else {
            // iterate through the file descriptors and check the event flags
            for ( int i = 0; i < nfds; i++ ) {
                int sockfd = fds[i].fd;
                if ( fds[i].revents & POLLIN ) {
                    struct sockaddr_in cli_addr;
                    socklen_t          clilen = sizeof(
                        cli_addr ); // store the size of the client adress
                    // accept create a new socket!
                    int newsockfd = accept(
                        sockfd, ( struct sockaddr * ) &cli_addr, &clilen );
                    if ( newsockfd < 0 ) {
                        // there was an error accepting the connection
                        std::cerr << "Error accepting connection" << std::endl;
                    } else {
                        this->read_request_data(newsockfd, 1024);
                        // handle request!
                        // send a response based on the request!
                        this->send_basic_response( newsockfd );
                        close( newsockfd );
                        // this->close_sockets_fd();
                    }
                }
            }
        }
    }
}

int Server::read_request_data( int socket, int request_size)
{
    char request_buf[request_size]; // buffer to store the request data
    int  bytes_received = recv( socket, request_buf, request_size, 0 );

    // check for errors
    if ( bytes_received < 0 ) {
        std::cerr << "Error reading from connection" << std::endl;
        return 1;
    }
    if (bytes_received == 0) {
        std::cerr << "Connection closed" << std::endl;
        return 1;
    }
    if (bytes_received > 0)
    {
        std::cout << "Request Buff" << std::endl << request_buf << std::endl;
    }
    return ( 0 );
}

int Server::handle_request_data() { return ( 0 ); }

int Server::send_basic_response( int socketfd )
{
    const char *response = "HTTP/1.1 200 OK\nContent-Type: "
                           "text/html\nContent-Length: 11\n\nHello World";
    if ( send( socketfd, response, strlen( response ), 0 ) == -1 ) {
        std::cout << "send: " << strerror( errno ) << std::endl;
        return ( 1 );
    }
    return ( 0 );
}

void Server::close_sockets_fd()
{
    std::vector<int>::iterator it;
    for ( it = this->sockets.begin(); it != this->sockets.end(); ++it ) {
        int sockfd = *it;
        close( sockfd );
    }
}

void Server::stop() 
{ 
    close_sockets_fd(); 
}

