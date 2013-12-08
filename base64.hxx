#ifndef INCLUDED_BASE64_HXX
#define INCLUDED_BASE64_HXX

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>

#include <string>

typedef boost::archive::iterators::transform_width <
		boost::archive::iterators::binary_from_base64 <
			std::string::iterator>,
		8, 6>
base64decodingIterator;

typedef boost::archive::iterators::base64_from_binary <
		boost::archive::iterators::transform_width <
			std::string::iterator, 6, 8>>
base64encodingIterator;

#endif


/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */


