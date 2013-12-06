#include "synchronicity.hxx"

#include <stdexcept>

std::ostream& operator<< (std::ostream& stream, const Synchronicity& s)
{
	switch (s) {
		case Synchronicity::ASYNCHRONE: 
			return stream << "ASynchrone";
			break;
		case Synchronicity::SYNCHRONE:
			return stream << "Synchrone";
			break;
		default:
			throw new std::logic_error("Unknown type of Synchronicity.");
	}
}


/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */


