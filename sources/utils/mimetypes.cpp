#include "mimetypes.hpp"

MimeType::MimeType()
{
    _mimeTypes[".css"] = "text/css";
    _mimeTypes[".csv"] = "text/csv";
    _mimeTypes[".doc"] = "application/msword";
    _mimeTypes[".docx"]
        = "application/"
          "vnd.openxmlformats-officedocument.wordprocessingml.document";
    _mimeTypes[".epub"]   = "application/epub+zip";
    _mimeTypes[".gz"]     = "application/gzip";
    _mimeTypes[".gif"]    = "image/gif";
    _mimeTypes[".htm"]    = "text/html";
    _mimeTypes[".html"]   = "text/html";
    _mimeTypes[".ico"]    = "image/vnd.microsoft.icon";
    _mimeTypes[".ics"]    = "text/calendar";
    _mimeTypes[".jpeg"]   = "image/jpeg";
    _mimeTypes[".jpg"]    = "image/jpeg";
    _mimeTypes[".js"]     = "text/javascript";
    _mimeTypes[".json"]   = "application/json";
    _mimeTypes[".jsonld"] = "application/ld+json";
    _mimeTypes[".mp3"]    = "audio/mpeg";
    _mimeTypes[".mpeg"]   = "video/mpeg";
    _mimeTypes[".png"]    = "image/png";
    _mimeTypes[".pdf"]    = "application/pdf";
    _mimeTypes[".php"]    = "application/x-httpd-php";
    _mimeTypes[".rar"]    = "application/vnd.rar";
    _mimeTypes[".svg"]    = "image/svg+xml";
    _mimeTypes[".txt"]    = "text/plain";
}

MimeType::~MimeType() {}

std::string MimeType::getMimeType( std::string extension )
{
    return _mimeTypes[extension];
}