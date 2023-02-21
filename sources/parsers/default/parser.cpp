#include "webserv.hpp"
#include <algorithm>

string handleKey( string &line ) { return line.substr( line.find( " " ) + 1 ); }

vector<string> split( string str, string delim )
{
    vector<string> result;
    size_t         pos = 0;
    string         token;
    while ( ( pos = str.find( delim ) ) != string::npos ) {
        token = str.substr( 0, pos );
        result.push_back( token );
        str.erase( 0, pos + delim.length() );
    }
    result.push_back( str );
    return result;
}

vector<string> handleVectorKey( string &line )
{
    return split( line.substr( line.find( " " ) + 1 ), " " );
}

string readFile( const string &fileName )
{
    ifstream ifs( fileName.c_str(), ios::in | ios::binary | ios::ate );

    ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg( 0, ios::beg );

    vector<char> bytes( fileSize );
    ifs.read( bytes.data(), fileSize );

    return string( bytes.data(), fileSize );
}

static inline string &ltrim( string &s )
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

static inline string &removeElements( string &s )
{
    s.erase( remove( s.begin(), s.end(), ';' ), s.end() );
    return s;
}

static inline string &innerTrim( string &input )
{
    string output;

    bool prev_was_space = false;
    for ( long unsigned int i = 0; i < input.size(); i++ ) {
        if ( input[i] == ' ' ) {
            if ( !prev_was_space && i > 0 && i < input.size() - 1 ) {
                output += ' ';
                prev_was_space = true;
            }
        } else {
            output += input[i];
            prev_was_space = false;
        }
    }

    input = output;

    return input;
}

string &trim( string &s )
{
    return removeElements( ltrim( rtrim( innerTrim( s ) ) ) );
}

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

    vector<vector<string> >  items;
    vector<string>::iterator it = lines.begin();

    while ( it != lines.end() && count ) {
        while ( it != lines.end() ) {
            if ( it->find( key ) != string::npos )
                break;
            ++it;
        }
        vector<string> item;

        while ( it != lines.end() ) {
            item.push_back( *it );
            ++it;
            if ( it == lines.end() || ( it->find( key ) != string::npos ) )
                break;
            if ( useBrackets && ( it->find( "}" ) != string::npos ) ) {
                item.push_back( *it );
                break;
            }
        }
        items.push_back( item );
        --count;
    }
    return items;
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

vector<Config> parseConfig( string filename )
{
    string                  file     = readFile( filename );
    vector<string>          lines    = readTidyLines( file );
    vector<string>          tmpLines = trimLines( lines );
    vector<vector<string> > servers  = splitLines( tmpLines, "server ", false );
    vector<vector<string> >::iterator it2 = servers.begin();
    vector<Config>                    serversConfig;

    while ( it2 != servers.end() ) {
        vector<string>::iterator it3 = it2->begin();
        vector<string>::iterator it4 = it2->end();
        Config                   config( it3, it4 );
        ++it2;
        serversConfig.push_back( config );
    }
    return serversConfig;
}
