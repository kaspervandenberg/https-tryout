#include <boost/network/protocol/http/client.hpp>
#include <boost/program_options.hpp>


#include <string>
#include <sstream>
#include <iostream>

namespace http = boost::network::http;
namespace po = boost::program_options;

int main(int argc, char * argv[]) {
	std::string host;
	std::string port;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "display this message")
		("host,s", po::value<std::string>(&host)->default_value("localhost"), "host to connect to.")
		("port,p", po::value<std::string>(&port)->default_value("80"), "port to listen on")
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
    
    try {
        http::client client;
        std::ostringstream url;
        url << "http://" << host << ":" << port << "/";

		for (int i=0; i<5; i++) {
			http::client::request request(url.str());
			http::client::response response =
				client.get(request);
			std::cout << body(response) << std::endl;
		}
    } catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}

/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */


