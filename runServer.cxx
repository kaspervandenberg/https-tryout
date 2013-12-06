#include "runServer.hxx"

#include "syncHttpEchoHandler.hxx"
#include "asyncHttpEchoHandler.hxx"
#include "syncHttp401Handler.hxx"

#include <boost/network/protocol/http/server.hpp>
#include <boost/network/utils/thread_pool.hpp>

namespace {

template <Handlers handler, Synchronicity sync>
struct TypeNames;

template <> struct TypeNames<Handlers::ECHO, Synchronicity::SYNCHRONE> {
	using Handler = SyncHttpEchoHandler;
	using Server = boost::network::http::server<Handler>;
	using Options = Server::options;
};

template <> struct TypeNames<Handlers::ECHO, Synchronicity::ASYNCHRONE> {
	using Handler = ASyncHttpEchoHandler;
	using Server = boost::network::http::async_server<Handler>;
	using Options = Server::options;
};

template <> struct TypeNames<Handlers::H401, Synchronicity::SYNCHRONE> {
	using Handler = SyncHttp401Handler;
	using Server = boost::network::http::server<Handler>;
	using Options = Server::options;
};


template <Handlers handler>
void runAsynchrone  (typename TypeNames<handler, Synchronicity::ASYNCHRONE>::Options& options)
{
	options.thread_pool(boost::make_shared<boost::network::utils::thread_pool>(2));
	typename TypeNames<handler, Synchronicity::ASYNCHRONE>::Server server (options);

	boost::thread t1(boost::bind(
			&boost::network::http::async_server<ASyncHttpEchoHandler>::run,
			&server));
	boost::thread t2(boost::bind(
			&boost::network::http::async_server<ASyncHttpEchoHandler>::run,
			&server));
	server.run();
	t1.join();
	t2.join();
}

template <Handlers handler>
void runSynchrone (typename TypeNames<handler, Synchronicity::SYNCHRONE>::Options& options)
{
	typename TypeNames<handler, Synchronicity::SYNCHRONE>::Server server (options);

	server.run();
}

template <Handlers handler, Synchronicity sync>
typename TypeNames<handler, sync>::Options setOptions(const std::string& port)
{
	typename TypeNames<handler, sync>::Handler handlerInstance;
	typename TypeNames<handler, sync>::Options options(handlerInstance);
	
	options.address("0.0.0.0")
			.port(port);

	return options;
}

} // end Namespace

void runServer(Synchronicity sync, Handlers handler, std::string port)
{
	if (handler == Handlers::ECHO && sync == Synchronicity::ASYNCHRONE)
	{
		auto options = setOptions<Handlers::ECHO, Synchronicity::ASYNCHRONE>(port);
		runAsynchrone<Handlers::ECHO>(options);
	}
	else if (handler == Handlers::ECHO && sync == Synchronicity::SYNCHRONE)
	{
		auto options = setOptions<Handlers::ECHO, Synchronicity::SYNCHRONE>(port);
		runSynchrone<Handlers::ECHO>(options);
	}
	else if (handler == Handlers::H401)
	{
		auto options = setOptions<Handlers::H401, Synchronicity::SYNCHRONE>(port);
		runSynchrone<Handlers::H401>(options);
	}
	else {
		throw new std::logic_error("Invallid settings");
	}
		

}

/* vim:set tabstop=4 shiftwidth=4 fo=cqwan autoindent : */
/* makeprg=make\ -C\ ~/tmp/build-try-https */


