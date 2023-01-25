/*
g++ -g3 -std=c++98 \
examples/parser/readlines.cpp sources/parsers/default/parser.cpp -I includes/
*/

#include "webserv.hpp"
#include <iostream>

static void testParseKey( vector<string> &lines, const string &key )
{
    vector<vector<string> > servers = splitLines( lines, key, false );

    vector<vector<string> >::iterator it2 = servers.begin();
    vector<Config>                    configs;
    while ( it2 != servers.end() ) {
        vector<string>::iterator it3 = it2->begin();
        vector<string>::iterator it4 = it2->end();
        Config                   config( it3, it4 );

        ++it2;
        configs.push_back( config );
    }
}

int main()
{
    vector<Config> conf = parseConfig( "config/config.conf" );
    return 0;
}
