#include "synchronicity.hxx"

#include <stdexcept>
#include <algorithm>
#include <cctype>

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

std::istream& operator>> (std::istream& stream, Synchronicity& s)
{
	std::string input;
	stream >> input;

	std::transform (input.begin(), input.end(), input.begin(), ::tolower);

	if (input == "asynchrone" || input == "async" || input == "asynchronous")
	{
		s = Synchronicity::ASYNCHRONE;
		return stream;
	}
	else if (input == "synchrone" || input == "sync" || input == "synchronous")
	{
		s = Synchronicity::SYNCHRONE;
		return stream;
	}
	
	throw new std::runtime_error(
			"Unsupported value for Synchronicity: "
			+ input);
}


/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */


