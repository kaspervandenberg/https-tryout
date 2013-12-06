#ifndef INCLUDED_ASYNC_HTTP_ECHO_HANDLER_HXX
#define INCLUDED_ASYNC_HTTP_ECHO_HANDLER_HXX

#include <boost/network/protocol/http/server.hpp>


struct ASyncHttpHandler
{
	typedef boost::network::http::async_server<ASyncHttpHandler> ASyncHttpServer;

	void operator()	(
			const ASyncHttpServer::request& request,
			ASyncHttpServer::connection_ptr connection);
};

#endif

/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */


