#include "Cgi_handler.hpp"

#define CGI_BUFSIZE 8192

Cgi_handler::Cgi_handler() {}

Cgi_handler::Cgi_handler( Request cgi_request ) : cgi_request( cgi_request )
{
    int         i;
    std::string test;
    this->create_env_vars();
    char **env_vars     = this->create_env_vars_array( this->env_vars );
    this->response_body = exec_cgi( this->cgi_request.cgi_path, env_vars );
    i                   = 0;
    while ( env_vars[i] ) {
        delete[] env_vars[i];
        i++;
    }
    delete[] env_vars;
    std::cout << this->response_body << std::endl;
}

Cgi_handler::~Cgi_handler() {}

/*The value of "SCRIPT_NAME" should be the URL path to the CGI script.
For example, if the script is located at
"http://example.com/cgi-bin/myscript.php" then the value of "SCRIPT_NAME" would
be "/cgi-bin/myscript.php". The value of "SCRIPT_FILENAME" should be the
absolute file system path to the CGI script. For example, if the script is
located at "/var/www/html/cgi-bin/myscript.php" then the value of
"SCRIPT_FILENAME" would be "/var/www/html/cgi-bin/myscript.php".
*/

void Cgi_handler::create_env_vars()
{

    this->env_vars["AUTH_TYPE"]         = cgi_request.authorization;
    this->env_vars["REDIRECT_STATUS"]   = "200";
    this->env_vars["GATEWAY_INTERFACE"] = "CGI/1.1";
    this->env_vars["SCRIPT_NAME"]       = cgi_request.cgi_path;
    this->env_vars["SCRIPT_FILENAME"]   = cgi_request.cgi_path;
    this->env_vars["REQUEST_METHOD"]    = cgi_request.method;
    this->env_vars["CONTENT_LENGTH"] = cgi_request.body; // Converter para char*
    this->env_vars["CONTENT_TYPE"]   = cgi_request.content_type;
    this->env_vars["PATH_INFO"]      = cgi_request.cgi_path;
    this->env_vars["PATH_TRANSLATED"] = cgi_request.cgi_path;
    this->env_vars["QUERY_STRING"]    = cgi_request.query;
    this->env_vars["REMOTEaddr"]      = cgi_request.port;
    this->env_vars["REMOTE_IDENT"]    = cgi_request.authorization;
    this->env_vars["REMOTE_USER"]     = cgi_request.authorization;
    this->env_vars["REQUEST_URI"] = cgi_request.cgi_path + cgi_request.query;
    this->env_vars["SERVER_NAME"] = cgi_request.host;
    this->env_vars["SERVER_PROTOCOL"] = "HTTP/1.1";
    this->env_vars["SERVER_SOFTWARE"] = "Weebserv/1.0";
}

char **Cgi_handler::create_env_vars_array(
    std::map<std::string, std::string> &env_vars )
{
    int    size   = env_vars.size();
    char **result = new char *[size + 1];
    std::map<std::string, std::string>::const_iterator it;
    int                                                i = 0;
    for ( it = env_vars.begin(); it != env_vars.end(); it++ ) {
        std::string env_var = it->first + "=" + it->second;
        result[i]           = new char[env_var.length() + 1];
        strcpy( result[i], env_var.c_str() );
        i++;
    }
    result[size] = 0;
    return result;
}

std::string Cgi_handler::exec_cgi( std::string cgi_script_path,
                                   char      **env_vars )
{
    int         pid;
    int         pipe_stdin[2];
    int         pipe_stdout[2];
    std::string new_body;

    if ( pipe( pipe_stdin ) == -1 || pipe( pipe_stdout ) == -1 ) {
        perror( "pipe" );
    }

    pid = fork();

    if ( pid == -1 ) {
        std::cerr << "Fork failed." << std::endl;
        return ( "Status: 500\r\n\r\n" );
    } else if ( !pid ) {
        // Child process
        close( pipe_stdin[1] );
        close( pipe_stdout[0] );

        dup2( pipe_stdin[0], STDIN_FILENO );
        dup2( pipe_stdout[1], STDOUT_FILENO );

        char *argv[] = { NULL };
        execve( cgi_script_path.c_str(), argv, env_vars );
        std::cerr << "Error: execve failed with error code: "
                  << strerror( errno ) << std::endl;
        write( STDOUT_FILENO, "Status: 500\r\n\r\n", 15 );
    } else {
        // Parent process
        close( pipe_stdin[0] );
        close( pipe_stdout[1] );

        // Write request body to pipe for stdin
        write(
            pipe_stdin[1], cgi_request.body.c_str(), cgi_request.body.size() );
        close( pipe_stdin[1] );

        // Read from pipe for stdout
        char buffer[CGI_BUFSIZE] = { 0 };
        int  ret                 = 1;
        while ( ret > 0 ) {
            memset( buffer, 0, CGI_BUFSIZE );
            ret = read( pipe_stdout[0], buffer, CGI_BUFSIZE - 1 );
            new_body += buffer;
        }
        close( pipe_stdout[0] );

        waitpid( -1, NULL, 0 );
    }

    if ( !pid )
        exit( 0 );

    return ( new_body );
}

std::string Cgi_handler::get_response_body() { return ( this->response_body ); }
