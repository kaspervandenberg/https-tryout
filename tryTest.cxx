#define BOOST_TEST_MODULE TryTest
#if !defined( WIN32 )
	#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( trivialEquals )
{
	BOOST_CHECK_EQUAL(1, 1);
}

BOOST_AUTO_TEST_CASE( trivialFailure )
{
/*	Having tests that always fail hinders testing,
	therefore this test is only enabled when
	EXECUTE_FAILING_TESTS is defined.
	Use:
	cmake -DTRY_TEST_EXECUTE_FAILING_TESTS=ON {source_path} && \
	make && \
	make test
	to enable the test.
*/
#if defined( EXECUTE_FAILING_TESTS )
	BOOST_CHECK_EQUAL(1, 0);
#endif
}

/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */


