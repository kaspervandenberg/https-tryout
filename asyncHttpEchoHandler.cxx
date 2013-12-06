#include "asyncHttpEchoHandler.hxx"

#include "echoRequest.hxx"

void ASyncHttpEchoHandler::operator() (
		const ASyncHttpServer::request& request,
		ASyncHttpServer::connection_ptr connection)
{
	
	std::shared_ptr<std::string> replyMessage { 
			new std::string { 
					echoRequest(Synchronicity::ASYNCHRONE, request) }};

	connection->set_status(ASyncHttpServer::connection::ok);
	
	int replySize = replyMessage->size();
	std::vector<ASyncHttpServer::response_header> headers {
		{ "Content-Type",  "text/text; charset=utf-8" },
		{ "Content-Length", std::to_string(replySize) }
	};
	connection->set_headers(boost::make_iterator_range(headers.begin(), headers.end()));

	std::cout << *replyMessage;

	connection->write(*replyMessage);
}

/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */


