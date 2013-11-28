#include "helloWorld.hxx"

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <boost/bind/arg.hpp>
#include <boost/program_options.hpp>

#include <openssl/ssl.h>
#include <openssl/safestack.h>

using namespace boost::asio;
using namespace boost::asio::ssl;
namespace po = boost::program_options;

typedef stream<ip::tcp::socket> ssl_socket;

SslFixedReplySession::SslFixedReplySession(io_service& io_, context& sslContext_) 
		: socket(io_, sslContext_) 
{
}


void SslFixedReplySession::start() {
	socket.async_handshake(boost::asio::ssl::stream_base::server,
		boost::bind(&SslFixedReplySession::handle_handShake, this, placeholders::error));
}


void SslFixedReplySession::handle_handShake(const boost::system::error_code& ec) {
	if(!ec) {
		write(socket, buffer("Hello World\n"));
		socket.async_shutdown(boost::bind(&SslFixedReplySession::handle_shutdown, this, placeholders::error));
	} else {
		std::cerr << "Handshake error occured: " << ec.message() << "\n";
		delete this;
	}
}


void SslFixedReplySession::handle_shutdown(const boost::system::error_code& ec) {
	if(!ec) {
		std::cout << "Shutdown succesful.\n";
	} else {
		if(ec == error::eof) {
			// Expected, ignore
		} else {
			std::cerr << "Error occured: " << ec.message();
		}
	}
	delete this;
}


ssl_socket::lowest_layer_type& SslFixedReplySession::socketType() {
	return socket.lowest_layer();
}




SslFixedReplyServer::SslFixedReplyServer(io_service& io_, int port, 
			const std::string& cert, const std::string& privateKey,
			const std::vector<std::string>& trustedCaCertificateFiles) 
		: io(io_),
			acceptor(io, ip::tcp::endpoint(ip::tcp::v4(), port)),
			sslContext(context::sslv3) {

	for(std::string caCertFile: trustedCaCertificateFiles) {
		addTrustedCaCertificate(caCertFile, true);
	}

	sslContext.set_verify_mode(verify_fail_if_no_peer_cert | verify_peer);
	sslContext.set_verify_callback(boost::bind(&SslFixedReplyServer::handle_verify, this, _1, _2));
	sslContext.set_password_callback(boost::bind(&SslFixedReplyServer::get_password, this));
	sslContext.use_certificate_file(cert, context::pem);
	sslContext.use_private_key_file(privateKey, context::pem);

	start_accept();
}

void SslFixedReplyServer::addTrustedCaCertificate(std::string fileName, bool addToClientCaList) {
	sslContext.load_verify_file(fileName);
	if (addToClientCaList) {
		STACK_OF(X509_NAME) *old_cert_names = SSL_CTX_get_client_CA_list(sslContext.native_handle());
		if(old_cert_names != nullptr && sk_X509_NAME_num(old_cert_names) > 0) {
			throw new std::logic_error("TODO: implement merging stacks of client CAs");
		}
		SSL_CTX_set_client_CA_list(sslContext.native_handle(), SSL_load_client_CA_file(fileName.c_str()));
	}
}

std::string SslFixedReplyServer::get_password() const {
	return "";
}


void SslFixedReplyServer::start_accept() {
	SslFixedReplySession* s = new SslFixedReplySession(io, sslContext);
	acceptor.async_accept(s->socketType(), 
		boost::bind(&SslFixedReplyServer::handle_accept, this, s, placeholders::error));
}


void SslFixedReplyServer::handle_accept(SslFixedReplySession* session, const boost::system::error_code& ec) {
	std::cout << "Accepted connection\n";
	if(!ec) {
		session->start();
	} else {
		std::cerr << "Error occured: " << ec.message();
		delete session;
	}

	start_accept();
}

bool SslFixedReplyServer::handle_verify(bool preverified, verify_context& context) {
	std::cout << "Server Verifying: preverified" << (preverified ? "true" : "false") << "\n";
	return preverified;
}


int main(int argc, char* argv[]) {
	int port;
	std::string certFile;
	std::string certPrivateKeyFile;
	std::vector<std::string> caFiles;

	try {

		po::options_description desc("Allowed options");
		desc.add_options()
			("help,h", "display this message")
			("port,p", po::value<int>(&port)->required(), "port to listen on")
			("certFile", po::value<std::string>(&certFile)->required(), "certificate to send as authentication")
			("certKey", po::value<std::string>(&certPrivateKeyFile)->required(), "private key for certificate")
			("caFile", po::value<std::vector<std::string> >(&caFiles), "List certificate files of trusted CA's")
		;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		if(vm.count("help")) {
			desc.print(std::cout);
			exit(0);
		}
		po::notify(vm);
	} catch (std::exception& ex) {
		std::cerr << ex.what() <<"\n";
		exit(1);
	}

	boost::asio::io_service io;
	SslFixedReplyServer server(io, port, certFile, certPrivateKeyFile, caFiles);
	std::cout << "Waiting...\n";

	io.run();
}



/* vim: set tabstop=4 shiftwidth=4 formatoptions=cqwan autoindent: */
