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
}

/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */

