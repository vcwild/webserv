#include "parsers.hpp"
#include <algorithm>

string readFile( const string &fileName )
{
    ifstream ifs( fileName.c_str(), ios::in | ios::binary | ios::ate );

    ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg( 0, ios::beg );

    vector<char> bytes( fileSize );
    ifs.read( bytes.data(), fileSize );

    return string( bytes.data(), fileSize );
}

static inline std::string &ltrim( std::string &s )
{
    s.erase(
        s.begin(),
        find_if( s.begin(), s.end(), not1( ptr_fun<int, int>( isspace ) ) ) );
    return s;
}

static inline string &rtrim( string &s )
{
    s.erase(
        find_if( s.rbegin(), s.rend(), not1( ptr_fun<int, int>( isspace ) ) )
            .base(),
        s.end() );
    return s;
}

static inline string &trim( string &s ) { return ltrim( rtrim( s ) ); }

vector<string> readLines( string &str )
{
    vector<string> lines;
    size_t         pos = 0;
    while ( ( pos = str.find( '\n' ) ) != string::npos ) {
        lines.push_back( str.substr( 0, pos ) );
        str.erase( 0, pos + 1 );
    }
    return lines;
}

unsigned int countKeys( vector<string> &lines, string key )
{
    vector<string>::iterator it    = lines.begin();
    unsigned int             count = 0;
    while ( it != lines.end() ) {
        if ( it->find( key ) != string::npos ) {
            ++count;
        }
        ++it;
    }
    return count;
}

vector<vector<string> >
splitLines( vector<string> &lines, string key, bool useBrackets )
{
    unsigned int count = countKeys( lines, key );
    if ( count == 1 )
        return vector<vector<string> >( 1, lines );

    vector<vector<string> >  servers;
    vector<string>::iterator it = lines.begin();

    while ( it != lines.end() && count ) {
        while ( it != lines.end() ) {
            if ( it->find( key ) != string::npos )
                break;
            ++it;
        }
        vector<string> server;

        while ( it != lines.end() ) {
            server.push_back( *it );
            ++it;
            if ( it == lines.end() || ( it->find( key ) != string::npos ) )
                break;
            if ( useBrackets && ( it->find( "}" ) != string::npos ) ) {
                server.push_back( *it );
                break;
            }
        }
        servers.push_back( server );
        --count;
    }
    return servers;
}

vector<string> trimLines( vector<string> &lines )
{
    vector<string>::iterator it = lines.begin();
    while ( it != lines.end() ) {
        trim( *it );
        if ( it->empty() ) {
            it = lines.erase( it );
        } else {
            ++it;
        }
    }
    return lines;
}

vector<string> readTidyLines( string &str )
{
    vector<string> lines = readLines( str );
    return trimLines( lines );
}
