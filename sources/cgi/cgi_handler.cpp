#include "cgi_handler.hpp"

#define CGI_BUFSIZE 8192

Cgi_handler::Cgi_handler() {}

Cgi_handler::Cgi_handler( Request _cgi_request ) : _cgi_request( _cgi_request )
{
    std::map<std::string, std::string> tmp_env_vars = _create_env_vars();

    _env_vars_array = _create_env_vars_array( tmp_env_vars );
    _response_body  = "";
}

Cgi_handler::~Cgi_handler()
{
    int i = 0;
    while ( _env_vars_array[i] ) {
        delete[] _env_vars_array[i];
        i++;
    }
    delete[] _env_vars_array;
}

void Cgi_handler::run()
{

    _response_body = exec_cgi( _cgi_request.cgi_path );

    std::cout << _response_body << std::endl;
}

/*The value of "SCRIPT_NAME" should be the URL path to the CGI script.
For example, if the script is located at
"http://example.com/cgi-bin/myscript.php" then the value of "SCRIPT_NAME" would
be "/cgi-bin/myscript.php". The value of "SCRIPT_FILENAME" should be the
absolute file system path to the CGI script. For example, if the script is
located at "/var/www/html/cgi-bin/myscript.php" then the value of
"SCRIPT_FILENAME" would be "/var/www/html/cgi-bin/myscript.php".
*/

std::map<std::string, std::string> Cgi_handler::_create_env_vars()
{
    std::map< std::string, std::string> tmp_env_vars;

    tmp_env_vars["AUTH_TYPE"]         = _cgi_request.authorization;
    tmp_env_vars["REDIRECT_STATUS"]   = "200";
    tmp_env_vars["GATEWAY_INTERFACE"] = "CGI/1.1";
    tmp_env_vars["SCRIPT_NAME"]       = _cgi_request.cgi_path;
    tmp_env_vars["SCRIPT_FILENAME"]   = _cgi_request.cgi_path;
    tmp_env_vars["REQUEST_METHOD"]    = _cgi_request.method;
    tmp_env_vars["CONTENT_LENGTH"]  = _cgi_request.body; // Converter para char*
    tmp_env_vars["CONTENT_TYPE"]    = _cgi_request.content_type;
    tmp_env_vars["PATH_INFO"]       = _cgi_request.cgi_path;
    tmp_env_vars["PATH_TRANSLATED"] = _cgi_request.cgi_path;
    tmp_env_vars["QUERY_STRING"]    = _cgi_request.query;
    tmp_env_vars["REMOTEaddr"]      = _cgi_request.port;
    tmp_env_vars["REMOTE_IDENT"]    = _cgi_request.authorization;
    tmp_env_vars["REMOTE_USER"]     = _cgi_request.authorization;
    tmp_env_vars["REQUEST_URI"] = _cgi_request.cgi_path + _cgi_request.query;
    tmp_env_vars["SERVER_NAME"] = _cgi_request.host;
    tmp_env_vars["SERVER_PROTOCOL"] = "HTTP/1.1";
    tmp_env_vars["SERVER_SOFTWARE"] = "Webserv/1.0";

    return tmp_env_vars;
}

char **Cgi_handler::_create_env_vars_array(
    std::map<std::string, std::string> env_vars )
{
    int    size   = env_vars.size();
    char **result = new char *[size + 1];
    std::map<std::string, std::string>::const_iterator it = env_vars.begin();

    for ( int i = 0; it != env_vars.end(); it++, i++ ) {
        std::string env_var = it->first + "=" + it->second;
        result[i]           = new char[env_var.length() + 1];
        strcpy( result[i], env_var.c_str() );
    }
    result[size] = 0;

    return result;
}

std::string Cgi_handler::exec_cgi( std::string cgi_script_path )
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
        execve( cgi_script_path.c_str(), argv, _env_vars_array );
        std::cerr << "Error: execve failed with error code: "
                  << strerror( errno ) << std::endl;
        write( STDOUT_FILENO, "Status: 500\r\n\r\n", 15 );
    } else {
        // Parent process
        close( pipe_stdin[0] );
        close( pipe_stdout[1] );

        // Write request body to pipe for stdin
        write( pipe_stdin[1],
               _cgi_request.body.c_str(),
               _cgi_request.body.size() );
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

    if ( !pid ) {
        int i = 0;
        while ( _env_vars_array[i] ) {
            delete[] _env_vars_array[i];
            i++;
        }
        delete[] _env_vars_array;
        exit( 0 );
    }

    return ( new_body );
}

std::string Cgi_handler::get_response_body() { return ( _response_body ); }
