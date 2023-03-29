#include "cgi_handler.hpp"
#include "minunit.h"
#include "webserv.hpp"

void test_setup() {}

void test_teardown() {}

Request createSampleRequest()
{
    Request req;
    req.method        = "GET";
    req.host          = "www.example.com";
    req.port          = "80";
    req.uri           = "/index.html";
    req.version       = "HTTP/1.1";
    req.body          = "";
    req.content_type  = "text/html";
    req.user_agent    = "Mozilla/5.0 (X11; Linux x86_64; rv:78.0)";
    req.authorization = "";
    req.query         = "name=value";
    req.cgi_path      = "./www/cgi-bin/python-cgi";
    return req;
}

MU_TEST( test_run_mock_cgi )
{
    Cgi_handler cgi( createSampleRequest() );
    cgi.run();
    std::string response = cgi.get_response_body();

    std::cout << response << std::endl;

    std::string expected
        = "<html>\n<head>\n<title>CGI Script</title>\n</head>\n<body>\n<h1>CGI "
          "Script</h1>\n<p>SCRIPT_NAME: \" + ./www/cgi-bin/python-cgi + "
          "\"</p>\n<p>PATH_INFO: \" + ./www/cgi-bin/python-cgi + "
          "\"</p>\n</body>\n</html>\n";

    std::size_t size = expected.size();

    std::string response_slice = response.substr( 0, size );

    mu_assert_string_eq( expected.c_str(), response_slice.c_str() );
}

MU_TEST( test_run_mock_cgi_wrong_path_should_return_status_500 )
{
    Request req  = createSampleRequest();
    req.cgi_path = "./www/cgi-bin/python-cgi-wrong-path";
    Cgi_handler cgi( req );
    cgi.run();
    std::string response = cgi.get_response_body();

    std::string expected = "Status: 500";

    std::size_t size = expected.size();

    std::string response_slice = response.substr( 0, size );

    mu_assert_string_eq( expected.c_str(), response_slice.c_str() );
}

MU_TEST_SUITE( suite_cgi )
{
    MU_RUN_TEST( test_run_mock_cgi );
    MU_RUN_TEST( test_run_mock_cgi_wrong_path_should_return_status_500 );
}

int main()
{
    MU_SUITE_CONFIGURE( &test_setup, &test_teardown );
    MU_RUN_SUITE( suite_cgi );
    MU_REPORT();
    return MU_EXIT_CODE;
}
