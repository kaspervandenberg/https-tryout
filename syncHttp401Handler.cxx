#include "syncHttp401Handler.hxx"

#include "echoRequest.hxx"
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/istream_iterator.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>

#include "base64.hxx"

namespace {
	namespace iter = boost::archive::iterators;

	void dumpString(std::string caption, std::string value)
	{
		std::cout << caption << ":\n";
		std::cout << "\t";
		for (auto ch: value) {
			std::cout << ch << "(" << int(ch) << ") ";
		}
		std::cout << "\n";
	}

	SyncHttp401Handler::SyncHttpServer::request::header_type findHeaderByName (
			std::string targetName,
			const SyncHttp401Handler::SyncHttpServer::request& request)
	{
		for (auto header: request.headers) {
			if (header.name == targetName) {
				return header;
			}
		}
		throw std::range_error ("Header named " + targetName + " not found");
	}

	void printAuthorizationHeaderParts (
			const SyncHttp401Handler::SyncHttpServer::request& request)
	{
		try
		{
			auto header = findHeaderByName ("Authorization", request);
			std::cout << "Header name: " << header.name << "\n";
			
			std::stringstream headerValue {header.value};
			std::string authorizationType;
			std::string base64data;
			headerValue >> authorizationType >> base64data;

			std::cout << "\t AuthorizationType: " << authorizationType << "\n"
					<< "\t data (base64): " << base64data << "\n";

			std::cout << "base64 length: " << base64data.size() << "\n\n";

			dumpString ("original", base64data);

			base64decodingIterator iDecodedStart { base64data.begin() };
			base64decodingIterator iDecodedEnd { base64data.end() };
			std::string decodedData { iDecodedStart, iDecodedEnd };

			dumpString ("decoded(1)", decodedData);

			base64encodingIterator iEncodedStart { decodedData.begin() };
			base64encodingIterator iEncodedEnd { decodedData.end() };

			std::string reencodedData = { iEncodedStart, iEncodedEnd };
			dumpString ("re-encoded", reencodedData);

			
			dumpString("Decoded twice",
					std::string (base64decodingIterator (reencodedData.begin()),
							base64decodingIterator (reencodedData.end())));
		}
		catch (const std::range_error& ex)
		{
			// Ignore,
			// No Authorization-header found so do not print anything.
		}
	}

}


void SyncHttp401Handler::operator()	(
		const SyncHttpServer::request& request,
		SyncHttpServer::response& response)
{
	using Header = SyncHttpServer::response_header;

	std::cout << "receveid request:\n";
	std::cout << echoRequest(Synchronicity::SYNCHRONE, request);
	std::cout << "\n<\\end request>\n\n";
	
	printAuthorizationHeaderParts(request);
	
	response = SyncHttpServer::response::stock_reply (
			SyncHttpServer::response::unauthorized,
			"<html><head><title>Demo: unathorized</title></head>\
			<body><h1>Demo: unathorized</h1>\
			This demo server always responds with 401 :: unauthorized\
			</bdoy></html>");
	response.headers.push_back(Header {
			"WWW-Authenticate",
			"Basic realm=\"test\""});
}


void SyncHttp401Handler::log(const SyncHttpServer::string_type& info)
{
	std::cerr << "ERROR " << info << '\n';
}


/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */


