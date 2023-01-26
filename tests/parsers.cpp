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
            mu_assert_string_eq( route.allow_methods[0].c_str(), "POST" );
            mu_assert_string_eq( route.allow_methods[1].c_str(), "GET" );
            mu_assert_string_eq( route.autoindex.c_str(), "off" );
            mu_assert_string_eq( route.cgi[0].c_str(), ".php" );
            mu_assert_string_eq( route.cgi[1].c_str(),
                                 "./www/cgi-bin/php-cgi" );
        }
        {
            Route route = config.routes[1];

            mu_assert_string_eq( route.location_dir.c_str(), "/teste2" );
            mu_assert_string_eq( route.allow_methods[0].c_str(), "POST" );
            mu_assert_string_eq( route.allow_methods[1].c_str(), "GET" );
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
            mu_assert_string_eq( route.allow_methods[0].c_str(), "POST" );
            mu_assert_string_eq( route.allow_methods[1].c_str(), "GET" );
            mu_assert_string_eq( route.autoindex.c_str(), "off" );
            mu_assert_string_eq( route.cgi[0].c_str(), ".php" );
            mu_assert_string_eq( route.cgi[1].c_str(),
                                 "./www/cgi-bin/php-cgi" );
        }
        {
            Route route = config.routes[1];

            mu_assert_string_eq( route.location_dir.c_str(), "/teste2" );
            mu_assert_string_eq( route.allow_methods[0].c_str(), "POST" );
            mu_assert_string_eq( route.allow_methods[1].c_str(), "GET" );
        }
    }
}

MU_TEST( test_check_get_request_parser )
{
    char    str[] = "GET / HTTP/1.1\r\nHost: localhost:9000\r\nUser-Agent: "
                    "curl/7.82.0\r\nAccept: */*\r\n\r\n";
    Request request( str );

    mu_assert_string_eq( request.method.c_str(), "GET" );
    mu_assert_string_eq( request.uri.c_str(), "/" );
    mu_assert_string_eq( request.version.c_str(), "HTTP/1.1" );
    mu_assert_string_eq( request.host.c_str(), "localhost" );
    mu_assert_string_eq( request.port.c_str(), "9000" );
    mu_assert_string_eq( request.user_agent.c_str(), "curl/7.82.0" );
    mu_assert_string_eq( request.accept.c_str(), "*/*" );
    mu_assert_string_eq( request.content_type.c_str(), "" );
    mu_assert_string_eq( request.content_length.c_str(), "" );
    mu_assert_string_eq( request.body.c_str(), "" );
}

MU_TEST( test_check_post_request_parser )
{
    char    str[] = "POST / HTTP/1.1\r\nHost: localhost:8000\r\nUser-Agent: "
                    "curl/7.82.0\r\nAccept: */*\r\nContent-Type: "
                    "application/json\r\nContent-Length: "
                    "34\r\n\r\n{\"key1\":\"value1\", \"key2\":\"value2\"}";
    Request request( str );

    mu_assert_string_eq( request.method.c_str(), "POST" );
    mu_assert_string_eq( request.uri.c_str(), "/" );
    mu_assert_string_eq( request.version.c_str(), "HTTP/1.1" );
    mu_assert_string_eq( request.host.c_str(), "localhost" );
    mu_assert_string_eq( request.port.c_str(), "8000" );
    mu_assert_string_eq( request.user_agent.c_str(), "curl/7.82.0" );
    mu_assert_string_eq( request.accept.c_str(), "*/*" );
    mu_assert_string_eq( request.content_type.c_str(), "application/json" );
    mu_assert_string_eq( request.content_length.c_str(), "34" );
    mu_assert_string_eq( request.body.c_str(),
                         "{\"key1\":\"value1\", \"key2\":\"value2\"}" );
}

MU_TEST( test_check_delete_request_parser )
{
    char str[]
        = "DELETE / HTTP/1.1\r\nHost: localhost:8000\r\nUser-Agent: "
          "curl/7.82.0\r\nAccept: */*\r\nContent-Length: 27\r\nContent-Type: "
          "application/"
          "x-www-form-urlencoded\r\n\r\nparam1=value1&param2=value2";
    Request request( str );

    mu_assert_string_eq( request.method.c_str(), "DELETE" );
    mu_assert_string_eq( request.uri.c_str(), "/" );
    mu_assert_string_eq( request.version.c_str(), "HTTP/1.1" );
    mu_assert_string_eq( request.host.c_str(), "localhost" );
    mu_assert_string_eq( request.port.c_str(), "8000" );
    mu_assert_string_eq( request.user_agent.c_str(), "curl/7.82.0" );
    mu_assert_string_eq( request.accept.c_str(), "*/*" );
    mu_assert_string_eq( request.content_type.c_str(),
                         "application/x-www-form-urlencoded" );
    mu_assert_string_eq( request.content_length.c_str(), "27" );
    mu_assert_string_eq( request.body.c_str(), "param1=value1&param2=value2" );
}

MU_TEST_SUITE( suite_name )
{
    MU_RUN_TEST( test_check_config_parser );
    MU_RUN_TEST( test_check_get_request_parser );
    MU_RUN_TEST( test_check_post_request_parser );
    MU_RUN_TEST( test_check_delete_request_parser );
}

int main()
{
    MU_SUITE_CONFIGURE( &test_setup, &test_teardown );
    MU_RUN_SUITE( suite_name );
    MU_REPORT();
    return MU_EXIT_CODE;
}
