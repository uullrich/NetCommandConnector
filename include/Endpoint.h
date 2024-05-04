#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <string>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/connect.hpp>

#include "../src/util/Buffer.h"

using boost::asio::awaitable;
using boost::asio::ip::tcp;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::steady_timer;

class CommandSender;

class ResponseReceivedHandler;

class Endpoint : public std::enable_shared_from_this<Endpoint> {
public:
    Endpoint(boost::asio::io_context &io_context, const std::string &ip, unsigned short port,
             ResponseReceivedHandler &responseHandler,
             std::function<void(std::shared_ptr<CommandSender>)> connectionEstablishedHandler);

    void send(const std::vector<unsigned char> &data);

    void send(const std::string &data);

private:
    awaitable<void> initConnection();

    awaitable<const tcp::resolver::iterator> resolve();

    awaitable<void> connect(const tcp::resolver::iterator &iterator);

    awaitable<void> onSendHandler();

    awaitable<void> onReceiveHandler();

    void disconnect();

    const std::string &_ip;
    unsigned short _port;

    tcp::socket _socket;
    tcp::resolver _resolver;
    steady_timer _timer;
    SendBuffer _sendBuffer;
    std::function<void(std::shared_ptr<CommandSender>)> _connectionEstablishedHandler;
    ResponseReceivedHandler &_responseHandler;
    ReceiveBuffer _receiveBuffer;
};

typedef std::shared_ptr<Endpoint> EndpointPtr;
#endif
