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
    req.cgi_path      = "./cgi-bin/python-cgi";
    return req;
}

MU_TEST( test_run_mock_cgi )
{
    Cgi_handler cgi( createSampleRequest() );
    cgi.run();
    std::string response = cgi.get_response_body();

    std::string expected = "Content-type: text/html\n";

    std::size_t size = expected.size();

    std::string response_slice = response.substr( 0, size );

    mu_assert_string_eq( expected.c_str(), response_slice.c_str() );
}

MU_TEST_SUITE( suite_cgi ) { MU_RUN_TEST( test_run_mock_cgi ); }

int main()
{
    MU_SUITE_CONFIGURE( &test_setup, &test_teardown );
    MU_RUN_SUITE( suite_cgi );
    MU_REPORT();
    return MU_EXIT_CODE;
}
