#include "response.hpp"

/*
When a client sends a POST request to a server, the server should respond with
an HTTP response that indicates the status of the request. The POST request is
used to submit data to the server, and the response from the server should
indicate whether the submission was successful or not.

If the POST request is successful, the server should return an HTTP response
with a status code of 200 (OK) or 201 (Created). If the data submitted in the
POST request has been successfully processed by the server and a new resource
has been created as a result of the submission, the server should return a
response with a status code of 201 and a Location header indicating the URL of
the newly created resource.

If the server is unable to process the data submitted in the POST request for
some reason, it should return an HTTP response with a status code that indicates
the reason for the failure. For example, if the data submitted is invalid, the
server should return a 400 (Bad Request) status code. If the client is not
authorized to submit the data, the server should return a 401 (Unauthorized) or
403 (Forbidden) status code.

Additionally, the server can include a response body with the HTTP response to
provide additional information about the outcome of the POST request. The
response body can be in various formats such as HTML, JSON, or XML depending on
the nature of the response.

In summary, when a client sends a POST request, the server should respond with
an HTTP response that indicates whether the submission was successful or not,
using an appropriate HTTP status code and optionally a response body that
provides additional information about the outcome of the request.
*/

void ft::Response::handlePost()
{
    // Content-type
    setStatusCode( "200 OK" );
    setBody( "POST" );
}