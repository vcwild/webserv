#include "logger.hpp"
#include "minunit.h"

void test_setup() {}

void test_teardown() {}

MU_TEST( test_log_level_debug )
{
    static Logger   logger;
    std::streambuf *stream = std::cout.rdbuf();
    std::ostream    out( stream );

    logger.setStream( out );

    logger.setLogLevel( "WARNING" );
    logger.debug( "This is a DEBUG message" );
    logger.error( "Hello friends!" );
    logger.info( "This is an INFO message" );
    logger.warning( "This is a WARNING message" );
    logger.error( "The server socket timed out" );
    logger.none( "This is a NONE message" );
}

MU_TEST_SUITE( suite_logger ) { MU_RUN_TEST( test_log_level_debug ); }

int main()
{
    MU_SUITE_CONFIGURE( &test_setup, &test_teardown );
    MU_RUN_SUITE( suite_logger );
    MU_REPORT();
    return MU_EXIT_CODE;
}
