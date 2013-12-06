#include <boost/network/protocol/http/server.hpp>
#include <boost/network/utils/thread_pool.hpp>

#include <boost/program_options.hpp>
#include <sstream>
#include <iostream>


#include "handlers.hxx"
#include "synchronicity.hxx"
#include "runServer.hxx"


namespace http = boost::network::http;
namespace po = boost::program_options;


//typedef http::server<SyncHttpHandler> SyncHttpServer;
//typedef http::async_server<ASyncHttpHandler> ASyncHttpServer;


int main(int argc, char * argv[])
{
	std::string port;
	Synchronicity synchronicity;
	Handlers handler;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "display this message")
		("port,p", po::value<std::string>(&port)->required(), "port to listen on")
		("async", po::value<Synchronicity>(&synchronicity)->default_value(Synchronicity::SYNCHRONE), "synchrous or asynchronous server")
		("handler", po::value<Handlers>(&handler)->default_value(Handlers::ECHO), "How to handle requests?")
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

	runServer(synchronicity, handler, port); 

	#if 0
	if (asyncServer) {
		ASyncHttpEchoHandler handler;

		http::async_server<ASyncHttpEchoHandler>::options options(handler);
		options.address("0.0.0.0")
				.port(port);
		options.thread_pool(boost::make_shared<boost::network::utils::thread_pool>(2));
		http::async_server<ASyncHttpEchoHandler> server (options);

		boost::thread t1(boost::bind(&http::async_server<ASyncHttpEchoHandler>::run, &server));
		boost::thread t2(boost::bind(&http::async_server<ASyncHttpEchoHandler>::run, &server));
		server.run();
		t1.join();
		t2.join();

	} else {
		SyncHttpEchoHandler handler;

		http::server<SyncHttpEchoHandler>::options options(handler);
		options.address("0.0.0.0")
				.port(port);
		http::server<SyncHttpEchoHandler> server (options);

		server.run();
	}
	#endif
}

/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */

