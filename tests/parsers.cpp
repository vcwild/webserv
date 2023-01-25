#include "parsers.hpp"
#include "minunit.h"
#include "webserv.hpp"

void test_setup() {}

void test_teardown() {}

MU_TEST( test_check_config_parser )
{
    vector<Config> conf = parseConfig( "config/config.conf" );
    {
        Config config = conf[0];

        mu_assert_int_eq( config.listen_port, 8000 );
        mu_assert_string_eq( config.server_name.c_str(), "localhost" );
        mu_assert_int_eq( config.client_max_body_size, 100 );
        mu_assert_string_eq( config.index[0].c_str(), "doesnotexist" );
        mu_assert_string_eq( config.index[1].c_str(), "index.html" );
        mu_assert_string_eq( config.allowed_method[0].c_str(), "GET" );
        mu_assert_string_eq( config.allowed_method[1].c_str(), "POST" );
        mu_assert_string_eq( config.autoindex.c_str(), "off" );
        mu_assert_string_eq( config.error_page[0].c_str(), "404" );
        mu_assert_string_eq( config.error_page[1].c_str(), "./404.html" );
        mu_assert_string_eq( config.root_dir.c_str(), "./www" );
        {
            Route route = config.routes[0];

            mu_assert_string_eq( route.location_dir.c_str(), "/teste" );
            mu_assert_string_eq( route.root.c_str(), "./root" );
            mu_assert_string_eq( route.index.c_str(), "teste.html" );
            mu_assert_string_eq( route.allowed_methods[0].c_str(), "POST" );
            mu_assert_string_eq( route.allowed_methods[1].c_str(), "GET" );
            mu_assert_string_eq( route.autoindex.c_str(), "off" );
            mu_assert_string_eq( route.cgi[0].c_str(), ".php" );
            mu_assert_string_eq( route.cgi[1].c_str(),
                                 "./www/cgi-bin/php-cgi" );
        }
        {
            Route route = config.routes[1];

            mu_assert_string_eq( route.location_dir.c_str(), "/teste2" );
            mu_assert_string_eq( route.allowed_methods[0].c_str(), "POST" );
            mu_assert_string_eq( route.allowed_methods[1].c_str(), "GET" );
        }
    }
    {
        Config config = conf[1];

        mu_assert_int_eq( config.listen_port, 9000 );
        mu_assert_string_eq( config.server_name.c_str(), "localhost" );
        mu_assert_int_eq( config.client_max_body_size, 100 );
        mu_assert_string_eq( config.index[0].c_str(), "doesnotexist" );
        mu_assert_string_eq( config.index[1].c_str(), "index.html" );
        mu_assert_string_eq( config.allowed_method[0].c_str(), "GET" );
        mu_assert_string_eq( config.allowed_method[1].c_str(), "POST" );
        mu_assert_string_eq( config.autoindex.c_str(), "off" );
        mu_assert_string_eq( config.error_page[0].c_str(), "404" );
        mu_assert_string_eq( config.error_page[1].c_str(), "./404.html" );
        mu_assert_string_eq( config.root_dir.c_str(), "./www" );
        {
            Route route = config.routes[0];

            mu_assert_string_eq( route.location_dir.c_str(), "/teste" );
            mu_assert_string_eq( route.root.c_str(), "./root" );
            mu_assert_string_eq( route.index.c_str(), "teste.html" );
            mu_assert_string_eq( route.allowed_methods[0].c_str(), "POST" );
            mu_assert_string_eq( route.allowed_methods[1].c_str(), "GET" );
            mu_assert_string_eq( route.autoindex.c_str(), "off" );
            mu_assert_string_eq( route.cgi[0].c_str(), ".php" );
            mu_assert_string_eq( route.cgi[1].c_str(),
                                 "./www/cgi-bin/php-cgi" );
        }
        {
            Route route = config.routes[1];

            mu_assert_string_eq( route.location_dir.c_str(), "/teste2" );
            mu_assert_string_eq( route.allowed_methods[0].c_str(), "POST" );
            mu_assert_string_eq( route.allowed_methods[1].c_str(), "GET" );
        }
    }
}

MU_TEST_SUITE( suite_name ) { MU_RUN_TEST( test_check_config_parser ); }

int main()
{
    MU_SUITE_CONFIGURE( &test_setup, &test_teardown );
    MU_RUN_SUITE( suite_name );
    MU_REPORT();
    return MU_EXIT_CODE;
}
