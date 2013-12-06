#ifndef INCLUDED_HANDLERS_HXX
#define INCLUDED_HANDLERS_HXX

#include <iostream>
#include "synchronicity.hxx"
#include "syncHttpEchoHandler.hxx"
#include "asyncHttpEchoHandler.hxx"

enum class Handlers {
	ECHO,
	H401
};

std::ostream& operator<< (std::ostream& stream, const Handlers& h);
std::istream& operator>> (std::istream& stream, Handlers& h);


#endif

/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */


