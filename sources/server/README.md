
The first argument, AF_INET, specifies the address family, which is set to Internet Protocol version 4 (IPv4). The second argument, SOCK_STREAM, specifies the type of socket to be created, which is a TCP socket. The third argument, 0, is for the protocol, which is set to 0 to use the default protocol for the given socket type.
```
int sockfd = socket( AF_INET, SOCK_STREAM, 0 );
if ( sockfd < 0 ) 
{
    std::cerr << "Error creating socket" << std::endl;
    return 1;
}
```

/*A non-blocking file descriptor is a file descriptor that allows the
process to continue executing while waiting for an I/O operation to
complete. This is in contrast to a blocking file descriptor, which
causes the process to block (stop execution) until the I/O operation is
completed. Non-blocking file descriptors are useful in situations where
the process needs to perform multiple tasks concurrently, as they allow
the process to perform other tasks while waiting for an I/O operation to
complete. They are often used in event-driven programming and can be set
using the fcntl function with the O_NONBLOCK flag.*/

```
 if ( fcntl( sockfd, F_SETFL, O_NONBLOCK ) < 0 ) {
    // there was an error setting the flags
    std::cerr << "Error setting flags for socket" << std::endl;
    return 1;
}
```