#include "Server.hpp"

static void signal_handler( int sign )
{
    std::cout << "Signal received: " << sign << std::endl;
}

Server::Server() : running( true ) {}

Server::Server( const Server &src ) { *this = src; }

Server &Server::operator=( const Server &src )
{
    ( void ) src;
    return ( *this );
}

Server::Server( std::vector<Config> servers_conf ) : running( true )
{
    servers_conf = servers_conf;
    start();
}

Server::~Server() {}

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
    }
    return 0;
}

void Server::accept_connections()
{
    int           n_fds = sockets.size();
    struct pollfd poll_fds[n_fds];
    int           timeout;

    signal( SIGINT, signal_handler );

    while ( running ) {
        for ( int i = 0; i < n_fds; i++ ) {
            poll_fds[i].fd     = sockets[i];
            poll_fds[i].events = POLLIN;
        }

        timeout = ( 3 * 60 * 1000 ); // 3 minutes

        int ret = poll( poll_fds, n_fds, timeout );

        if ( ret < 0 ) {
            // there was an error with the poll function
            std::cerr << "Error with poll function" << std::endl;
            break;
        }

        if ( ret == 0 ) {
            // the poll function timed out
            std::cout << "Poll timed out" << std::endl;
            break;
        }

        // iterate through the file descriptors and check the event flags
        for ( int i = 0; i < n_fds; i++ ) {
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
                } else {
                    read_request_data( connection_socket, 1024 );
                    std::cout << "Request Buff" << std::endl
                              << requests[connection_socket] << std::endl;
                    // handle request!
                    // send a response based on the request!
                    send_basic_response( connection_socket );
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

int Server::send_basic_response( int socketfd )
{
    const char *response   = "HTTP/1.1 200 OK\nContent-Type: "
                             "text/html\nContent-Length: 11\n\nHello World";
    int         bytes_sent = send( socketfd, response, strlen( response ), 0 );

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
    std::vector<int>::iterator it;
    for ( it = sockets.begin(); it != sockets.end(); ++it ) {
        int sockfd = *it;
        close( sockfd );
    }

    std::map<int, std::string>::iterator it2;
    for ( it2 = requests.begin(); it2 != requests.end(); it2++ ) {
        close( it2->first );
    }

    requests.clear();
}

void Server::stop() { close_sockets_fd(); }
