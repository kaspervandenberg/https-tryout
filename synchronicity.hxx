#ifndef INCLUDED_SYNCHRONICITY_HXX
#define INCLUDED_SYNCHRONICITY_HXX

#include <iostream>

enum class Synchronicity {
	ASYNCHRONE,
	SYNCHRONE
};

std::ostream& operator<< (std::ostream& stream, const Synchronicity& s);


#endif

/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */


