#include <iostream>
#include <memory.h>
#include <netdb.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void err( int err, const char *msg )
{
    std::cerr << msg << ": " << gai_strerror( err ) << std::endl;
    exit( 1 );
}

int main()
{
    struct addrinfo hints, *res, *res0;
    int             error;
    int             s;
    const char     *cause = NULL;

    memset( &hints, 0, sizeof( hints ) );
    hints.ai_family   = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    error             = getaddrinfo( "www.kame.net", "http", &hints, &res0 );
    if ( error ) {
        err( 1, "getaddrinfo" );
        /*NOTREACHED*/
    }
    s = -1;
    for ( res = res0; res; res = res->ai_next ) {
        s = socket( res->ai_family, res->ai_socktype, res->ai_protocol );
        if ( s < 0 ) {
            cause = "socket";
            continue;
        }

        if ( connect( s, res->ai_addr, res->ai_addrlen ) < 0 ) {
            cause = "connect";
            close( s );
            s = -1;
            continue;
        }

        break; /* okay we got one */
    }
    if ( s < 0 ) {
        err( 1, cause );
        /*NOTREACHED*/
    }
    freeaddrinfo( res0 );
}
