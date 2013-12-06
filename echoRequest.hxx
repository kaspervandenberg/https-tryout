#ifndef INCLUDED_ECHO_REQUEST_HXX
#define INCLUDED_ECHO_REQUEST_HXX

#include "synchronicity.hxx"

#include <string>
#include <sstream>

template <typename TRequest>
std::string echoRequest (const Synchronicity& synchronicity, const TRequest& request)
{
	std::stringstream responseBody;

	responseBody << "<body>";
	responseBody << "Received " << synchronicity << " request.:\n";

	responseBody << "Headers:\n";
	auto headers_range = request.headers;
	for (auto header: headers_range) {
		responseBody << "\t" << header.name << ":" << header.value << "\n";
	}
	responseBody << "\n";
	responseBody << "Destination: " << request.destination << "\n";
	responseBody << "Body:\n" << request.body <<"\n\n";
	responseBody << "</body>\n";
	

	return responseBody.str();
}

#endif

/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */


