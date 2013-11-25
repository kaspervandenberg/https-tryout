#ifndef INCLUDED_HELLO_WORLD_HXX
#define INCLUDED_HELLO_WORLD_HXX

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/system/error_code.hpp>

class SslFixedReplySession;

/**
 * A server that communicates over ssl and sends a fixed reply to each request.
 * 
 * The server requires the files:
 * - serv-cert.crt, an ssl-certificate; and
 * - serv-priv.pem, a private key without a password
 *
 * Start the server via:
 * > ./HelloWorld {port_nr}
 *
 * Use OpenSSL to connect to the server:
 * > openssl s_client -connect localhost:{port_nr}
 */
class SslFixedReplyServer {
public:
	SslFixedReplyServer(boost::asio::io_service& io_, int port); 

	/**
	 * Callback to supply password to asio
	 */
	std::string get_password() const; 

	/**
	 * Listen and accept a connection
	 */
	void start_accept(); 

	/**
	 * Callback to handle accept,
	 * replies with fixed response.
	 */
	void handle_accept(SslFixedReplySession* session, const boost::system::error_code& ec); 

private:
	boost::asio::io_service& io;
	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::ssl::context sslContext;
};

/**
 * Serves a single session
 */
class SslFixedReplySession {
public:
	SslFixedReplySession(boost::asio::io_service& io_, boost::asio::ssl::context& sslContext_);

	/**
	 * The SslFixedReplyServer starts the session from its handle_accept.
	 */
	void start(); 

	/**
	 * When asio has finished the handshake, this callback sends the actual 
	 * reply.
	 */
	void handle_handShake(const boost::system::error_code& ec); 

	/**
	 * Callback for after closing the connection.
	 */
	void handle_shutdown(const boost::system::error_code& ec); 

	boost::asio::ssl::stream<boost::asio::ip::tcp::socket>::lowest_layer_type& socketType(); 

private:
	boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket;

};

#endif
/* vim: set tabstop=4 shiftwidth=4 formatoptions=cqwan autoindent: */
