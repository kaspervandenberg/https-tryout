#include <boost/network/protocol/http/server.hpp>
#include <boost/network/utils/thread_pool.hpp>

#include <boost/program_options.hpp>
#include <sstream>
#include <iostream>

#include "syncHttpEchoHandler.hxx"
#include "asyncHttpEchoHandler.hxx"


namespace http = boost::network::http;
namespace po = boost::program_options;


typedef http::server<SyncHttpHandler> SyncHttpServer;
typedef http::async_server<ASyncHttpHandler> ASyncHttpServer;


int main(int argc, char * argv[])
{
	std::string port;
	bool asyncServer;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "display this message")
		("port,p", po::value<std::string>(&port)->required(), "port to listen on")
		("async", po::bool_switch(&asyncServer), "use asynchronous server")
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

