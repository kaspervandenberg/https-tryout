#include "syncHttpEchoHandler.hxx"
#include "echoRequest.hxx"

void SyncHttpEchoHandler::operator()	(
		const SyncHttpServer::request& request,
		SyncHttpServer::response& response)
{
	response = SyncHttpServer::response::stock_reply(
			SyncHttpServer::response::ok,
			echoRequest(Synchronicity::SYNCHRONE, request));
}
	
void SyncHttpEchoHandler::log(const SyncHttpServer::string_type& info)
{
	std::cerr << "ERROR " << info << '\n';
}

/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */

