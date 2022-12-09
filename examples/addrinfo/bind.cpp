#include <arpa/inet.h>
#include <iostream>
#include <memory.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

#define MY_PORT 5555

int main()
{
    int                socket_fd;
    struct sockaddr_in server_address;

    socket_fd = socket( AF_INET, SOCK_STREAM, 0 );

    server_address.sin_family      = AF_INET;
    server_address.sin_port        = htons( MY_PORT );
    server_address.sin_addr.s_addr = inet_addr( "10.12.110.57" );
    memset( server_address.sin_zero, '\0', sizeof( server_address.sin_zero ) );

    bind( socket_fd,
          ( struct sockaddr * ) &server_address,
          sizeof( server_address ) );
    return 0;
}
