#define BOOST_TEST_MODULE test_module_tracker
#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <boost/test/included/unit_test.hpp>
#include <boost/test/unit_test.hpp>

// entry point:
int main(int argc, char* argv[], char* envp[])
{
    return boost::unit_test::unit_test_main(init_unit_test, argc, argv);
}