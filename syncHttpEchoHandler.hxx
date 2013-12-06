#ifndef INCLUDED_SYNC_HTTP_ECHO_HANDLER_HXX
#define INCLUDED_SYNC_HTTP_ECHO_HANDLER_HXX_

#include <boost/network/protocol/http/server.hpp>

struct SyncHttpHandler
{
	typedef boost::network::http::server<SyncHttpHandler> SyncHttpServer;

	void operator()	(
			const SyncHttpServer::request& request,
			SyncHttpServer::response& response);
	
	void log(const SyncHttpServer::string_type& info);
};


#endif

/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */

