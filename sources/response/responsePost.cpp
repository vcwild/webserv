#include "cgi_handler.hpp"
#include "response.hpp"

std::string ft::Response::generateFileName()
{
    std::string rootDir = server_conf.root_dir;

    std::string file_path = rootDir + "/uploads/";

    for ( int i = 0; i < 10; i++ ) {
        file_path += static_cast<char>( rand() % 26 + 97 );
    }

    file_path += ".txt";
    return file_path;
}
bool isMultipartFormData(const std::string& content_type) {
    return content_type.find("multipart/form-data") != std::string::npos;
}

std::string getMultipartFormDataBoundary(const std::string& content_type) {
    std::string boundary_string("boundary=");
    std::string::size_type pos = content_type.find(boundary_string);

    if (pos == std::string::npos) {
        return "";
    }

    pos += boundary_string.length();
    return content_type.substr(pos);
}


std::map<std::string, std::string> extractMultipartFormDataParts(const std::string& body, const std::string& boundary) {
    std::map<std::string, std::string> parts;
    std::string::size_type pos = 0;
    std::string::size_type boundary_len = boundary.length();

    while (pos < body.length()) {
        std::string::size_type start_pos = body.find(boundary, pos);
        if (start_pos == std::string::npos) {
            break;
        }

        start_pos += boundary_len;

        std::string::size_type end_pos = body.find(boundary, start_pos);
        if (end_pos == std::string::npos) {
            break;
        }

        std::string part_data = body.substr(start_pos, end_pos - start_pos - 2);

        std::string::size_type content_disp_pos = body.find("Content-Disposition: form-data;", pos);
        if (content_disp_pos != std::string::npos) {
            std::string::size_type name_pos = body.find("name=\"", content_disp_pos);
            std::string::size_type name_end_pos = body.find("\"", name_pos + 6);
            if (name_pos != std::string::npos && name_end_pos != std::string::npos) {
                std::string part_name = body.substr(name_pos + 6, name_end_pos - name_pos - 6);
                parts["name"] = part_name;
            }

            std::string::size_type filename_pos = body.find("filename=\"", content_disp_pos);
            std::string::size_type filename_end_pos = body.find("\"", filename_pos + 10);
            if (filename_pos != std::string::npos && filename_end_pos != std::string::npos) {
                std::string part_filename = body.substr(filename_pos + 10, filename_end_pos - filename_pos - 10);
                parts["filename"] = part_filename;
            }

            std::string::size_type content_type_pos = body.find("Content-Type:", content_disp_pos);
            std::string::size_type content_type_end_pos = body.find("\r\n", content_type_pos + 13);
            if (content_type_pos != std::string::npos && content_type_end_pos != std::string::npos) {
                std::string part_content_type = body.substr(content_type_pos + 13, content_type_end_pos - content_type_pos - 13);
                parts["content_type"] = part_content_type;
            }

            // Remove header information from part data
            std::string::size_type data_pos = part_data.find("\r\n\r\n");
            if (data_pos != std::string::npos) {
                parts["data"] = part_data.substr(data_pos + 4);
            }
        }

        pos = end_pos + boundary_len;
    }

    return parts;
}


bool createFileFromFormData(const std::map<std::string, std::string>& parts, const std::string& root_dir) {
    std::string name = parts.at("name");
    std::string filename = parts.at("filename");
    std::string data = parts.at("data");

    std::string file_path = root_dir + "/uploads/" + filename;

    // Check if the file already exists
    std::ifstream existing_file(file_path.c_str());
    if (existing_file.good()) {
        existing_file.close();
        return false;
    }

    std::ofstream file(file_path.c_str(), std::ios::out | std::ios::binary);
    if (file.is_open()) {
        file.write(data.c_str(), data.length());
        file.close();
        return true;
    } else {
        logger.error("Error creating file on server: " + file_path);
        return false;
    }
}

void ft::Response::handlePost()
{
    std::string path = getPath( request.uri );

    if ( request.cgi_path != "" ) {
        Cgi_handler handler( request );
        handler.run();
        setStatusCode( status_codes.getStatusCode( 200 ) );
        setContentType( mime_types.getMimeType( ".html" ) );
        setBody( handler.get_response_body() );
    }
    else
    {
        if (isMultipartFormData(request.content_type)) {

        std::string boundary = getMultipartFormDataBoundary(request.content_type);
        if (!boundary.empty()) 
        {
            std::map<std::string, std::string> parts = extractMultipartFormDataParts(request.body, boundary);

            // Check if the uploaded file is of the correct type
            if (parts.count("content_type") > 0 && parts["content_type"].find("text/plain") != std::string::npos) {
                // Create the file if it's of the correct type
               bool created = createFileFromFormData(parts, server_conf.root_dir);

                if (created) {
                    setStatusCode(status_codes.getStatusCode(201));
                    setContentType(mime_types.getMimeType(".html"));
                    setBody("<html><body><h1>201 Created</h1></body></html>");
                } else {
                    setStatusCode(status_codes.getStatusCode(409));
                    setContentType(mime_types.getMimeType(".html"));
                    setBody("<html><body><h1>409 Conflict</h1><p>A file with the same name already exists.</p></body></html>");
                }
            } else {
                // If the uploaded file is of the wrong type, return a 400 Bad Request error
                setStatusCode(status_codes.getStatusCode(400));
                setContentType(mime_types.getMimeType(".html"));
                setBody("<html><body><h1>400 Bad Request</h1></body></html>");
            }

        } else {
            // If it's not a multipart/form-data request with a valid boundary, return a 400 Bad Request error
            setStatusCode(status_codes.getStatusCode(400));
            setContentType(mime_types.getMimeType(".html"));
            setBody("<html><body><h1>400 Bad Request</h1></body></html>");
        }
    } 
        else 
        {
            // If it's not a multipart/form-data request, return a 202 Accepted status code indicating the request is received but not processed
            setStatusCode(status_codes.getStatusCode(202));
            setContentType(mime_types.getMimeType(".html"));
            setBody("<html><body><h1>202 Accepted</h1></body></html>");
        }
    }

}
