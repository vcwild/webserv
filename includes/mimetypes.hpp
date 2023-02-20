#ifndef MIME_HPP
#define MIME_HPP

#include <map>
#include <string>

class MimeType {

public:
    MimeType();
    ~MimeType();
    std::string getMimeType( std::string extension );

private:
    std::map<std::string, std::string> _mimeTypes;
};

#endif
