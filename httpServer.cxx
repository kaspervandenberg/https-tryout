#include <boost/network/protocol/http.hpp>
#include <boost/network/protocol/http/server.hpp>


// using namespace boost::network;
namespace http = boost::network::http;

struct HttpHandler;

typedef http::server<HttpHandler> HttpServer;

struct HttpHandler
{
	void operator()	(
			const HttpServer::request& request,
			HttpServer::response& response)
	{
		response = HttpServer::response::stock_reply(
				HttpServer::response::ok, "Hello World (HTTP)\n");
	}
	

	void log(const HttpServer::string_type& info)
	{
		std::cerr << "ERROR " << info << '\n';
	}
};


int main(int argc, char * argv[])
{
	HttpHandler handler;
	HttpServer::options options(handler);
	HttpServer server (
			options.address("0.0.0.0")
					.port("8080"));
	server.run();
}

/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */

