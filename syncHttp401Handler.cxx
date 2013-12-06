#include "syncHttp401Handler.hxx"

#include "echoRequest.hxx"
#include <iostream>

void SyncHttp401Handler::operator()	(
		const SyncHttpServer::request& request,
		SyncHttpServer::response& response)
{
	using Header = SyncHttpServer::response_header;

	std::cout << "receveid request:\n";
	std::cout << echoRequest(Synchronicity::SYNCHRONE, request);
	std::cout << "\n<\\end request>\n\n";
	


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


