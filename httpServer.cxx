#include <boost/network/protocol/http/server.hpp>
#include <boost/program_options.hpp>

namespace http = boost::network::http;
namespace po = boost::program_options;


struct HttpHandler;

typedef http::server<HttpHandler> SyncHttpServer;
typedef http::async_server<HttpHandler> ASyncHttpServer;

struct HttpHandler
{
	void operator()	(
			const SyncHttpServer::request& request,
			SyncHttpServer::response& response)
	{
		response = SyncHttpServer::response::stock_reply(
				SyncHttpServer::response::ok, "Hello World (HTTP)\n");
	}
	
	void operator()	(
			const ASyncHttpServer::request& request,
			ASyncHttpServer::connection_ptr connection)
	{
		
//								= ASyncHttpServer::response::stock_reply(
//				ASyncHttpServer::response::ok, "Hello World (ASync-HTTP)\n");
//			connection->write(response);
	}
	

	void log(const SyncHttpServer::string_type& info)
	{
		std::cerr << "ERROR " << info << '\n';
	}
};


int main(int argc, char * argv[])
{
	int port;
	bool asyncServer;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "display this message")
		("port,p", po::value<int>(&port)->required(), "port to listen on")
		("async", po::value<bool>(&asyncServer)->default_value(false), "use asynchronous server")
	;

	try {


		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		if(vm.count("help")) {
			desc.print(std::cout);
			exit(0);
		}
		po::notify(vm);
	} catch (std::exception& ex) {
		std::cerr << "Missing required option: \n";
		std::cerr << ex.what() << "\n";
		
		std::cerr << "Useage:\n";
		desc.print(std::cerr);

		exit(1);
	}

	HttpHandler handler;

	SyncHttpServer::options options(handler);
	options.address("0.0.0.0")
			.port("8080");

	asyncServer = false;
	if (asyncServer) {
//		ASyncHttpServer server (options);
//		server.run();
	} else {
		SyncHttpServer server (options);
		server.run();
	}
}

/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */

