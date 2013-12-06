#include "handlers.hxx"

#include <stdexcept>
#include <algorithm>
#include <cctype>

std::ostream& operator<< (std::ostream& stream, const Handlers& handler)
{
	switch (handler) {
		case Handlers::ECHO:
			return stream << "ECHO";
			break;
		case Handlers::H401:
			return stream << "401";
			break;
		default:
			throw new std::logic_error("Unknown type of Handlers.");
	}
}

std::istream& operator>> (std::istream& stream, Handlers& handler)
{
	std::string input;
	stream >> input;

	std::transform (input.begin(), input.end(), input.begin(), ::tolower);

	if(input == "echo")
	{
		handler = Handlers::ECHO;
		return stream;
	}
	else if (input == "401" || input == "h401")
	{
		handler = Handlers::H401;
		return stream;
	}
	
	throw new std::runtime_error(
			"Unsupported value for Handlers: "
			+ input);
}


/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */


