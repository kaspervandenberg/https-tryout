#include <boost/network/protocol/http/server.hpp>
#include <boost/network/utils/thread_pool.hpp>

#include <boost/program_options.hpp>

namespace http = boost::network::http;
namespace po = boost::program_options;


struct SyncHttpHandler;
struct ASyncHttpHandler;

typedef http::server<SyncHttpHandler> SyncHttpServer;
typedef http::async_server<ASyncHttpHandler> ASyncHttpServer;

struct SyncHttpHandler
{
	void operator()	(
			const SyncHttpServer::request& request,
			SyncHttpServer::response& response)
	{
		response = SyncHttpServer::response::stock_reply(
				SyncHttpServer::response::ok, "Hello World (HTTP)\n");
	}
	
	void log(const SyncHttpServer::string_type& info)
	{
		std::cerr << "ERROR " << info << '\n';
	}
};

struct ASyncHttpHandler
{
	void operator()	(
			const ASyncHttpServer::request& request,
			ASyncHttpServer::connection_ptr connection)
	{
		
//								= ASyncHttpServer::response::stock_reply(
//				ASyncHttpServer::response::ok, "Hello World (ASync-HTTP)\n");
//			connection->write(response);
	}
};


int main(int argc, char * argv[])
{
	std::string port;
	bool asyncServer;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "display this message")
		("port,p", po::value<std::string>(&port)->required(), "port to listen on")
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

	if (asyncServer) {
		ASyncHttpHandler handler;

		ASyncHttpServer::options options(handler);
		options.address("0.0.0.0")
				.port(port);
		options.thread_pool(boost::make_shared<boost::network::utils::thread_pool>(2));
		ASyncHttpServer server (options);

		boost::thread t1(boost::bind(&ASyncHttpServer::run, &server));
		boost::thread t2(boost::bind(&ASyncHttpServer::run, &server));
		server.run();
		t1.join();
		t2.join();

	} else {
		SyncHttpHandler handler;

		SyncHttpServer::options options(handler);
		options.address("0.0.0.0")
				.port(port);
		SyncHttpServer server (options);

		server.run();
	}
}

/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */

