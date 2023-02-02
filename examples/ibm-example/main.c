/*
 * This example was coded based from the IBM
 * https://www.ibm.com/docs/en/i/7.3?topic=designs-example-nonblocking-io-select
 */

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>

#define PORT 5000

#define TRUE 1
#define FALSE 0

static check_error( int rc, char *msg, int listen_sd )
{
    if ( rc < 0 ) {
        perror( msg );
        close( listen_sd );
        exit( -1 );
    }
}

static close_nfds( int nfds, struct pollfd fds[] )
{
    int i;
    for ( i = 0; i < nfds; i++ ) {
        if ( fds[i].fd >= 0 ) {
            printf( "  Closing fd: %d ", fds[i].fd );
            close( fds[i].fd );
        }
    }
}

int main( int argc, char *arg[] )
{
    int                 len, rc, on = 1;
    int                 listen_sd = -1, new_sd = -1;
    int                 desc_ready, end_server = FALSE, compress_array = FALSE;
    int                 close_conn;
    char                buffer[80];
    struct sockaddr_in6 addr;
    int                 timeout;
    struct pollfd       fds[200];
    int                 nfds = 1, current_size = 0, i, j;

    // Create the listening socket as IPv6 socket
    listen_sd = socket( AF_INET6, SOCK_STREAM, 0 );
    if ( listen_sd < 0 ) {
        perror( "socket() failed" );
        exit( -1 );
    }

    // Allow socket descriptor to be reuseable
    rc = setsockopt(
        listen_sd, SOL_SOCKET, SO_REUSEADDR, ( char * ) &on, sizeof( on ) );
    check_error( rc, "setsockopt() failed", listen_sd );

    // Set socket to be nonblocking. All of the sockets for the incoming
    rc = ioctl( listen_sd, FIONBIO, ( char * ) &on );
    check_error( rc, "ioctl() failed", listen_sd );

    // Bind the socket
    memset( &addr, 0, sizeof( addr ) );
    addr.sin6_family = AF_INET6;
    memcpy( &addr.sin6_addr, &in6addr_any, sizeof( in6addr_any ) );
    addr.sin6_port = htons( PORT );
    rc = bind( listen_sd, ( struct sockaddr * ) &addr, sizeof( addr ) );
    check_error( rc, "bind() failed", listen_sd );

    // Set the listen back log
    rc = listen( listen_sd, 32 );
    check_error( rc, "listen() failed", listen_sd );

    // Initialize the pollfd structure
    memset( fds, 0, sizeof( fds ) );

    // Set up the initial listening socket
    fds[0].fd     = listen_sd;
    fds[0].events = POLLIN;

    // Initialize the timeout to 3 minutes. End after 3 minutes
    timeout = ( 3 * 60 * 1000 );

    // Loop waiting for incoming connects or for incoming data on any of the
    // connected sockets
    do {
        // Call poll() and wait 3 minutes for it to complete
        printf( "Waiting on poll()... \n" );
        rc = poll( fds, nfds, timeout );

        // Check to see if the poll call failed
        if ( rc < 0 ) {
            perror( "  poll() failed" );
            break;
        }

        // Check to see if the 3 minute time out expired
        if ( rc == 0 ) {
            printf( "  poll() timed out. End program.\n" );
            break;
        }

        // One or more descriptors are readable. Need to determine which ones
        // they are
        current_size = nfds;

        for ( i = 0; i < current_size; i++ ) {
            // Loop through to find the descriptors that returned POLLIN
            if ( fds[i].revents == 0 )
                continue;

            // If revents is not POLLIN, it's an unexpected result, log and end
            if ( fds[i].revents != POLLIN ) {
                printf( "  Error! revents = %d\n", fds[i].revents );
                end_server = TRUE;
                break;
            }

            // Check to see if the POLLIN event is on the listening socket
            if ( fds[i].fd == listen_sd ) {
                printf( "  Listening socket is readable\n" );

                // Accept all incoming connections that are queued up on the
                // listening socket before we loop back and call poll again
                do {
                    new_sd = accept( listen_sd, NULL, NULL );
                    if ( new_sd < 0 ) {
                        if ( errno != EWOULDBLOCK ) {
                            perror( "  accept() failed" );
                            end_server = TRUE;
                        }
                        break;
                    }

                    printf( "  New incoming connection - %d\n", new_sd );

                    // Add the new incoming connection to the pollfd structure
                    fds[nfds].fd     = new_sd;
                    fds[nfds].events = POLLIN;
                    nfds++;

                    // Loop back up and accept another incoming connection
                } while ( new_sd != -1 );
            }

            // If the POLLIN event is on a connected socket, then its an
            // indication that incoming data is available
            else {
                printf( "  Descriptor %d is readable\n", fds[i].fd );
                close_conn = FALSE;

                // Receive all incoming data on this socket before we loop back
                // and call poll again
                do {
                    // Receive data on this connection until the recv fails with
                    // EWOULDBLOCK. If any other failure occurs, we will close
                    // the connection
                    rc = recv( fds[i].fd, buffer, sizeof( buffer ), 0 );
                    if ( rc < 0 ) {
                        if ( errno != EWOULDBLOCK ) {
                            perror( "  recv() failed" );
                            close_conn = TRUE;
                        }
                        break;
                    }

                    // Check to see if the connection has been closed by the
                    // client
                    if ( rc == 0 ) {
                        printf( "  Connection closed\n" );
                        close_conn = TRUE;
                        break;
                    }

                    // Data was received
                    len = rc;
                    printf( "  %d bytes received\n", len );

                    // Echo the data back to the client
                    rc = send( fds[i].fd, buffer, len, 0 );
                    if ( rc < 0 ) {
                        perror( "  send() failed" );
                        close_conn = TRUE;
                        break;
                    }
                } while ( TRUE );

                // If the close_conn flag was turned on, we need to clean up
                // this active connection. This clean up process includes
                // removing the descriptor from the pollfd structure and
                // closing the descriptor
                if ( close_conn ) {
                    close( fds[i].fd );
                    fds[i].fd      = -1;
                    compress_array = TRUE;
                }
            }
        }

        // If the compress_array flag was turned on, we need to squeeze together
        // the array and decrement the number of file descriptors. We do not
        // need to move back the events and revents fields because the events
        // will always be POLLIN in this case, and revents is output.
        if ( compress_array ) {
            compress_array = FALSE;
            for ( i = 0; i < nfds; i++ ) {
                if ( fds[i].fd == -1 ) {
                    for ( j = i; j < nfds; j++ ) {
                        fds[j].fd = fds[j + 1].fd;
                    }
                    i--;
                    nfds--;
                }
            }
        }
    } while ( end_server == FALSE );

    // Clean up all of the sockets that are open
    close_nfds( fds, nfds );
}