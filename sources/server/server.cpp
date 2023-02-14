#include "server.hpp"

Server::Server() {}

Server::~Server()
{
    if ( !sockets.empty() ) {
        close_sockets_fd();
    }
}

Server::Server( const Server &src ) { *this = src; }

Server &Server::operator=( const Server &src )
{
    ( void ) src;
    return ( *this );
}

Server::Server( std::vector<Config> servers_conf )
{
    this->servers_conf = servers_conf;
    start();
}

int Server::start()
{
    create_sockets();
    accept_connections();
    close_sockets_fd();
    return ( TRUE );
}

/*
This create sockest for it server of the conf file.
*/
int Server::create_sockets()
{
    std::vector<Config>::iterator it;
    for ( it = servers_conf.begin(); it != servers_conf.end(); ++it ) {
        const Config &server_conf = *it;

        // create a new socket
        int sockfd = socket( AF_INET, SOCK_STREAM, 0 );
        if ( sockfd < 0 ) {
            std::cerr << "Error creating socket" << std::endl;
            return FALSE;
        }

        if ( fcntl( sockfd, F_SETFL, O_NONBLOCK ) < 0 ) {
            // there was an error setting the flags
            std::cerr << "Error setting flags for socket" << std::endl;
            return FALSE;
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
            return FALSE;
        }

        if ( listen( sockfd, SOMAXCONN ) < 0 ) {
            std::cerr << "Error setting socket to listen" << std::endl;
            return FALSE;
        }

        // add the socket file descriptor to the vector
        sockets.push_back( sockfd );
        printf( "Socket created at port %d with fd %d \n",
                server_conf.listen_port,
                sockfd );
    }
    return 0;
}

void Server::accept_connections()
{
    int           n_fds = sockets.size();
    struct pollfd poll_fds[n_fds];
    int           timeout;

    printf( "Accepting connections on %d sockets \n", n_fds );

    while ( true ) {
        for ( int i = 0; i < n_fds; i++ ) {
            poll_fds[i].fd     = sockets[i];
            poll_fds[i].events = POLLIN;
        }

        timeout = ( 3 * 60 * 1000 ); // 3 minutes

        int ret = poll( poll_fds, n_fds, timeout );

        if ( ret < 0 ) {
            // there was an error with the poll function
            std::cerr << "Error with poll function" << std::endl;
            exit( -1 );
            break;
        }

        if ( ret == 0 ) {
            // the poll function timed out
            std::cout << "Poll timed out" << std::endl;
            exit( -1 );
            break;
        }

        // iterate through the file descriptors and check the event flags
        for ( int i = 0; i < n_fds; i++ ) {
            printf( "Polling socket" );
            int server_socket = poll_fds[i].fd;
            if ( poll_fds[i].revents & POLLIN ) {
                struct sockaddr_in cli_addr;
                // store the size of the client adress
                socklen_t clilen = sizeof( cli_addr );

                // accept create a new socket!
                int connection_socket = accept(
                    server_socket, ( struct sockaddr * ) &cli_addr, &clilen );
                if ( connection_socket < 0 ) {
                    // there was an error accepting the connection
                    std::cerr << "Error accepting connection" << std::endl;
                    close( server_socket );
                } else {
                    read_request_data( connection_socket, 1024 );
                    std::cout << "Request Buff" << std::endl
                              << requests[connection_socket] << std::endl;
                    // handle request!
                    // send a response based on the request!
                    Request   request( requests[connection_socket].c_str() );
                    ResponseC response( request );
                    send_response( connection_socket, response );
                    close( connection_socket );
                }
            }
        }
    }
    return;
}

int Server::read_request_data( int socket, int request_size )
{
    char request_buf[request_size]; // buffer to store the request data
    int  bytes_received = recv( socket, request_buf, request_size, 0 );

    // check for errors
    if ( bytes_received < 0 ) {
        std::cerr << "Error reading from connection" << std::endl;
        close( socket );
        return FALSE;
    }

    if ( bytes_received == 0 ) {
        std::cerr << "Connection closed" << std::endl;
        close( socket );
        return FALSE;
    }

    if ( bytes_received > 0 ) {
        requests[socket] = request_buf;
    }

    return ( TRUE );
}

int Server::handle_request_data() { return ( 0 ); }

int Server::send_response( int socketfd, ResponseC res )
{
    // const char *response   = res.makeResponse();
    std::cout << "Sending response" << res.getContentLength() << std::endl;
    std::string response = res.makeResponse();

    int bytes_sent = send( socketfd, response.c_str(), response.length(), 0 );

    if ( bytes_sent == -1 ) {
        std::cout << "send: -1 error" << std::endl;
        close( socketfd );
        return ( FALSE );
    }

    if ( bytes_sent == 0 ) {
        std::cout << "send: 0 error" << std::endl;
        close( socketfd );
        return ( FALSE );
    }

    return ( TRUE );
}

void Server::close_sockets_fd()
{
    printf( "Closing sockets \n" );
    std::map<int, std::string>::iterator it2;
    for ( it2 = requests.begin(); it2 != requests.end(); it2++ ) {
        printf( "Closing socket %d \n", it2->first );
        close( it2->first );
    }

    std::vector<int>::iterator it;
    for ( it = sockets.begin(); it != sockets.end(); ++it ) {
        int sockfd = *it;
        printf( "Closing socket %d \n", sockfd );
        close( sockfd );
    }

    requests.clear();
}
