/*
g++ -g3 -std=c++98 \
examples/parser/readlines.cpp sources/parsers/default/parser.cpp -I includes/
*/

#include "parsers.hpp"
#include <iostream>

static void testParseKey( vector<string> &lines, const string &key )
{
    cout << "Parsing key: " << key << endl;
    vector<vector<string> > servers = splitLines( lines, key, true );

    vector<vector<string> >::iterator it2 = servers.begin();

    while ( it2 != servers.end() ) {
        vector<string>::iterator it3 = it2->begin();
        while ( it3 != it2->end() ) {
            cout << *it3 << endl;
            ++it3;
        }
        cout << "------------------" << endl;
        ++it2;
    }
}

int main()
{
    string file = readFile( "config/config.conf" );
    // split newlines as new strings

    vector<string> lines = readTidyLines( file );

    vector<string> tmpLines = trimLines( lines );

    testParseKey( lines, "server {" );
    testParseKey( lines, "location " );

    return 0;
}
