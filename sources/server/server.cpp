#include "server.hpp"

static void check_error( int rc, std::string msg, int listen_sd )
{
    if ( rc < 0 ) {
        logger.error( msg );
        close( listen_sd );
        exit( -1 );
    }
}

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
            logger.error( "Error creating socket" );
            return FALSE;
        }

        int on = 1; // used for setsockopt
        int rc
            = setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof( on ) );

        check_error( rc, "bind() failed", sockfd );

        if ( fcntl( sockfd, F_SETFL, O_NONBLOCK ) < 0 ) {
            // there was an error setting the flags
            logger.error( "Error setting flags for socket" );
            return FALSE;
        }

        // bind the socket to the port specified in the configuration file
        struct sockaddr_in serv_addr;
        serv_addr.sin_family      = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port        = htons( server_conf.listen_port );
        // set socket options reuse address
        if ( bind(
                 sockfd, ( struct sockaddr * ) &serv_addr, sizeof( serv_addr ) )
             < 0 ) {
            logger.error( "Error binding socket to port "
                          + NumberToString( server_conf.listen_port ) );
            return FALSE;
        }

        if ( listen( sockfd, SOMAXCONN ) < 0 ) {
            logger.error( "Error setting socket to listen" );
            return FALSE;
        }

        // add the socket file descriptor to the vector
        sockets.push_back( sockfd );
        logger.info( "Server started on port "
                     + NumberToString( server_conf.listen_port ) );
    }
    return 0;
}

void Server::accept_connections()
{
    int           n_fds = sockets.size();
    struct pollfd poll_fds[n_fds];
    int           timeout;

    while ( true ) {
        for ( int i = 0; i < n_fds; i++ ) {
            poll_fds[i].fd     = sockets[i];
            poll_fds[i].events = POLLIN;
        }

        timeout = ( 3 * 60 * 1000 ); // 3 minutes

        int ret = poll( poll_fds, n_fds, timeout );

        if ( ret < 0 ) {
            // there was an error with the poll function
            logger.error( "Error with poll function" );
            exit( -1 );
            break;
        }

        if ( ret == 0 ) {
            // the poll function timed out
            logger.info( "Poll timed out" );
            exit( -1 );
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
                    logger.error( "Error accepting connection" );
                    close( server_socket );
                } else {
                    read_request_data( connection_socket, 1024 );
                    Request      request( requests[connection_socket].c_str() );
                    ft::Response response( request );
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
        logger.error( "Error reading from connection" );
        close( socket );
        return FALSE;
    }

    if ( bytes_received == 0 ) {
        logger.error( "Connection closed" );
        close( socket );
        return FALSE;
    }

    if ( bytes_received > 0 ) {
        requests[socket] = request_buf;
    }

    return ( TRUE );
}

int Server::handle_request_data() { return ( 0 ); }

int Server::send_response( int socketfd, ft::Response res )
{
    std::string response = res.makeResponse();

    int bytes_sent = send( socketfd, response.c_str(), response.length(), 0 );

    if ( bytes_sent == -1 ) {
        logger.error( "Error sending response (-1)" );
        close( socketfd );
        return ( FALSE );
    }

    if ( bytes_sent == 0 ) {
        logger.error( "Error sending response (0)" );
        close( socketfd );
        return ( FALSE );
    }

    return ( TRUE );
}

void Server::close_sockets_fd()
{
    std::map<int, std::string>::iterator it2;
    for ( it2 = requests.begin(); it2 != requests.end(); it2++ ) {
        close( it2->first );
    }

    std::vector<int>::iterator it;
    for ( it = sockets.begin(); it != sockets.end(); ++it ) {
        int sockfd = *it;
        close( sockfd );
    }

    requests.clear();
}
