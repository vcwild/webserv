#include "response.hpp"

/*
When a client sends a DELETE request to a server, the server should respond with
an HTTP response that indicates the status of the request. The DELETE request is
used to delete a resource on the server, and the response from the server should
indicate whether the deletion was successful or not.

If the DELETE request is successful, the server should return an HTTP response
with a status code of 200 (OK) or 204 (No Content). If the resource has been
successfully deleted, the server should return a response with a status code of
200 and, optionally, a response body that provides additional information about
the deleted resource.

If the server is unable to delete the resource for some reason, it should return
an HTTP response with a status code that indicates the reason for the failure.
For example, if the resource being deleted does not exist, the server should
return a 404 (Not Found) status code. If the client is not authorized to delete
the resource, the server should return a 401 (Unauthorized) or 403 (Forbidden)
status code.

It's important to note that the DELETE request is an idempotent request, meaning
that making multiple DELETE requests to the same resource should have the same
effect as making a single request. Therefore, if a client sends a DELETE request
and receives a successful response, it should be safe to assume that the
resource has been deleted and not send additional DELETE requests.

In summary, when a client sends a DELETE request, the server should respond with
an HTTP response that indicates whether the deletion was successful or not,
using an appropriate HTTP status code and, optionally, a response body.
*/

static void readFromAFile( std::string path, std::string &body )
{
    std::ifstream file( path.c_str() );
    if ( file.is_open() ) {
        std::string line;
        while ( getline( file, line ) ) {
            body.append( line );
        }
        file.close();
    }
}

void ft::Response::handleDelete()
{
    std::string   index_path = getPath( request.uri );
    std::ifstream index_file( index_path.c_str() );

    if ( index_file.is_open() ) 
    {
        std::string error_path = getPath( "/401.html" );
        readFromAFile( error_path, body );
        setStatusCode( status_codes.getStatusCode( 401 ) );
        index_file.close(); 
    }
    else
    {
        std::string error_path = getPath( "/404.html" );
        readFromAFile( error_path, body );
        setStatusCode( status_codes.getStatusCode( 404 ) );
    }
    
}