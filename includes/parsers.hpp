#ifndef PARSERS_H
#define PARSERS_H
#include <fstream>
#include <string>
#include <vector>

/* Definitions */
#define LISTEN_PORT_KEY "listen"
#define SERVER_NAME_KEY "server_name"
#define CLIENT_MAX_BODY_SIZE_KEY "client_max_body_size"
#define INDEX_KEY "index"
#define ALLOWED_METHOD_KEY "allowed_method"
#define ERROR_PAGE_KEY "error_page"
#define AUTOINDEX_KEY "autoindex"
#define ROUTE_KEY "location"
#define ROOT_KEY "root"
#define CGI_KEY "cgi"
#define HTTP_REDIRECTION_KEY "http_redirection"
#define ALLOW_METODS_KEY "allow_methods"

#define HOST_KEY "Host"
#define CONTENT_LENGTH_KEY "Content-Length"
#define CONTENT_TYPE_KEY "Content-Type"
#define USER_AGENT_KEY "User-Agent"
#define AUTHORIZATION_KEY "Authorization"
#define ACCEPT_KEY "Accept"

#define STATUS_KEY "Status"
#define ACCEPT_RANGES_KEY "Accept-Ranges"
#define ALLOW_KEY "Allow"
#define CONTENT_ENCODING_KEY "Content-Encoding"
#define CONTENT_LANGUAGE_KEY "Content-Language"
#define CONTENT_LOCATION_KEY "Content-Location"
#define CONTENT_RANGE_KEY "Content-Range"
#define CONTENT_TYPE_KEY "Content-Type"
#define DATE_KEY "Date"
#define SERVER_KEY "Server"
#define CONNECTION_KEY "Connection"
#define KEEP_ALIVE_KEY "Keep-Alive"
#define CGI_PATH "/cgi-bin/"

/* METHODS */
#define GET "GET"
#define POST "POST"
#define PUT "PUT"
#define DELETE "DELETE"

using namespace std;

std::string &removeSpecialCharacters( std::string       &str,
                                      const std::string &chars );

std::string &findSubstring( std::string &str, const std::string &chars );

string handleKey( string &line );

vector<string> split( string str, string delim );

vector<string> handleVectorKey( string &line );

string         readFile( const string &fileName );
string        &trim( string &s );
vector<string> readLines( string &str );
vector<string> trimLines( vector<string> &lines );
vector<vector<string> >
splitLines( vector<string> &lines, string key, bool useBrackets );

/**
 * @brief Read a string and return a vector of strings with trimmed lines.
 *
 * @param str string buffer to read
 * @return vector<string> vector of strings
 */
vector<string> readTidyLines( string &str );

#endif
