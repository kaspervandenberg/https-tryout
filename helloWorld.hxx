#ifndef INCLUDED_HELLO_WORLD_HXX
#define INCLUDED_HELLO_WORLD_HXX

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/system/error_code.hpp>
#include <string>
#include <vector>

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
 * > openssl s_client -connect localhost:{port_nr} -CAfile {root CA cert}
 * > openssl s_client -connect localhost:{port_nr} \
 *   -CAfile {root CA cert} -key {private key} -cert {signed cert}
 */
class SslFixedReplyServer {
public:
	/**
	 * Construct an SslFixedReplyServer.
	 * @param port 		where this server listens for incomming requests;
	 * @param cert 		path of the certificate file that the server
	 *		authenticates itself with;
	 * @param privateKey	path of the private key used to unlock cert; and
	 * @param trustedCaCertifcates		vector of paths to trusted CA's.
	 */
	SslFixedReplyServer(
			boost::asio::io_service& io_, int port,
			const std::string& cert, const std::string& privateKey,
			const std::vector<std::string>& trustedCaCertifcates);

	/**
	 * Add the certificate to the trusted CA certificates.
	 * @param fileName	path to PEM file containing certificate
	 * @addTrustedCaCertificate		true: send all CA names in the file to the
	 *			client during handshake;
	 *		false: only add the CA's to the trusted CA but do not send these
	 *			CA's to the client.
	 */
	void addTrustedCaCertificate(std::string fileName, bool addToClientCaList);

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

	/**
	 * Callback to verify certifacates
	 */
	bool handle_verify(bool preverified, boost::asio::ssl::verify_context& context);

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
