### Como um servidor http funciona?

Um servidor http funciona recebendo requests da internet e enviando de volta uma resposta. As requisições são feitas usando o protocolo de comunicação http (que é uma maneira padrão que a informação é trocada). 

Quando o usuário entra em um site, o navegador envia um requisição http para o servidor, e o servidor envia uma resposta. Geralmente a resposta é uma página da Web ou outro tipo de resposta como arquivos.

O servidor http funciona usando sockets. Os sockets são como uma porta virtual que conecta o servidor com diferentes tipos de serviços. Os sockets são usandos em nosso servidor para estabelecer a conexão do servidor em uma determinada porta e host no qual vários clientes vão enviar um request. Além disso, quando o cliente faz uma requisição, o nosso servidor cria um novo socket, aceitando a conexão. 

### I/O Multiplexing

O I/O multiplexing é uma técnica que permite o programa monitorar multiplos inputs e outputs de file descriptors ou sockets em único processo ou em uma única thread ao invés de ter que trabalhar lidando com um por vez. Isso é útil quando o programa precisa receber várias conexões de uma única vez. 

No nosso programa nós utilizamos a função poll() para lidar com isso.

De forma simplificada, quando chamamos a função  nós pegamos uma lista de sockets (socketsfd) e esperamos até que um ou mais desses sockets estejam prontos para ler ou enviar informações. 

Depois que o poll é chamado ele indica quais sockest estão prontos para fazer o I/O. 

Nós começamos criando um struct que será usado pela função poll:

```
struct pollfd poll_fds[n_fds];
```

Depois nós preenchemos esse struct usando os fds dos sockets abertos pelo nosso servidor:

```
for ( int i = 0; i < n_fds; i++ ) {
        poll_fds[i].fd     = this->sockets[i];
        poll_fds[i].events = POLLIN;
    }
```

Nos eventos, colocamos qual tipo de eventos nós estamos interessados. Nesse caso coloquei POLLIN porque queremos os eventos de input. 

Depois disso nós chamamos a função poll. Se ela retornar um valor maior que zero, significa que um ou mais eventos ocorreram dentro do poll_fds. Depois disso cheamos o reevents field para cada filedescriptor. Se o reenvento tiver a flag POLLIN, isso significa que ele possui informações para serem lidas.

Depois disso, nós usamos accept para criar uma socket para conectar com os clientes e ler as informações requisitadas. 


Check open file descriptors:
valgrind --track-fds=yes ./bin/webserv ./config/config.conf


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