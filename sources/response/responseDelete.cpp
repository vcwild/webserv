#include "response.hpp"
#include <cstdio> // For std::remove

static void readFromAFile(std::string path, std::string &body)
{
    std::ifstream file(path.c_str());
    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            body.append(line);
        }
        file.close();
    }
}

void ft::Response::handleDelete()
{
    std::string index_path = getPath(request.uri);
    std::ifstream index_file(index_path.c_str());

    if (index_file.is_open())
    {
        index_file.close();
        if (std::remove(index_path.c_str()) == 0) 
        {
            setStatusCode(status_codes.getStatusCode(204));
            setContentType(mime_types.getMimeType(".html"));
            setBody("<html><body><h1>204 No Content</h1></body></html>");
        }
        else 
        {
            std::string error_path = getPath("/500.html");
            readFromAFile(error_path, body);
            setStatusCode(status_codes.getStatusCode(500));
            setContentType(mime_types.getMimeType(".html"));
            setBody("<html><body><h1>500 Internal Server Error</h1></body></html>");
        }
    }
    else
    {
        std::string error_path = getPath("/404.html");
        readFromAFile(error_path, body);
        setContentType(mime_types.getMimeType(".html"));
        setStatusCode(status_codes.getStatusCode(404));
        setBody("<html><body><h1>404 Not Found</h1></body></html>");
    }
}
