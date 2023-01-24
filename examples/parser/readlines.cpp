/*
g++ -g3 -std=c++98 \
examples/parser/readlines.cpp sources/parsers/default/parser.cpp -I includes/
*/

#include "parsers.hpp"
#include <iostream>

int main()
{
    string file = readFile( "config/config.conf" );
    // split newlines as new strings

    vector<string> lines = readTidyLines( file );

    vector<string> tmpLines = trimLines( lines );

    vector<string>::iterator it = tmpLines.begin();

    vector<vector<string> > servers = splitLines( lines, "server {", true );

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

    return 0;
}
