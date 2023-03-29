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



void ft::Response::handlePost()
{
    if (isMultipartFormData(request.content_type)) {
    // handle multipart/form-data request
    std::cout << "Multipart form data" << std::endl;

    std::string boundary = getMultipartFormDataBoundary(request.content_type);
    if (!boundary.empty()) {
        std::cout << "Boundary: " << boundary << std::endl;
     std::map<std::string, std::string> parts = extractMultipartFormDataParts(request.body, boundary);
    std::string name = parts["name"];
    std::string filename = parts["filename"];
    std::string content_type = parts["content_type"];
    std::string data = parts["data"];

    std:: cout << "Name: " << name << std::endl;
    std:: cout << "Filename: " << filename << std::endl;
    std:: cout << "Content-Type: " << content_type << std::endl;
    std:: cout << "Data: " << data << std::endl;

    } else {
        // handle other types of request
    }
    } else {
        // handle other types of request
    }
    /*
    std::string path = getPath( request.uri );

    if ( request.content_type != "text/plain" ) {
        setStatusCode( status_codes.getStatusCode( 200 ) );
        return;
    }

    std::string   file_path = generateFileName();
    std::ofstream file( file_path.c_str() );

    if ( !file.is_open() ) {
        setStatusCode( status_codes.getStatusCode( 500 ) );
        setContentType( mime_types.getMimeType( ".html" ) );
        setBody(
            "<html><body><h1>500 Internal Server Error</h1></body></html>" );
        return;
    }

    file << request.body;
    file.close();
    setStatusCode( status_codes.getStatusCode( 201 ) );
    setContentType( mime_types.getMimeType( ".html" ) );
    setBody( "<html><body><h1>201 Created</h1></body></html>" );
    */
}