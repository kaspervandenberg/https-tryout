#include "helloWorld.hxx"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>

using namespace boost::asio;
using namespace boost::asio::ssl;

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




SslFixedReplyServer::SslFixedReplyServer(io_service& io_, int port) 
		: io(io_),
		acceptor(io, ip::tcp::endpoint(ip::tcp::v4(), port)),
		sslContext(context::sslv3) {
	sslContext.set_verify_mode(boost::asio::ssl::verify_none);
	sslContext.set_password_callback(boost::bind(&SslFixedReplyServer::get_password, this));
	sslContext.use_certificate_chain_file("serv-cert.crt");
	sslContext.use_private_key_file("serv-priv.pem", context::pem);

	start_accept();
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


int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Usage HelloWorld {port_nr}\n";
		exit(1);
	}
	boost::asio::io_service io;
	SslFixedReplyServer server(io, std::atoi(argv[1]));
	std::cout << "Waiting...\n";

	io.run();
}

/* vim: set tabstop=4 shiftwidth=4 formatoptions=cqwan autoindent: */
