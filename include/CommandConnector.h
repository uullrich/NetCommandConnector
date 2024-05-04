#ifndef COMMAND_CONNECTOR_H
#define COMMAND_CONNECTOR_H

#include <string>
#include <functional>
#include "Endpoint.h"
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>

using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;

class CommandSender;

class ResponseReceivedHandler;

class CommandConnector {
public:
    CommandConnector(const std::string &ip, unsigned short port, ResponseReceivedHandler &responseHandler,
                     std::function<void(std::shared_ptr<CommandSender>)> connectionEstablishedHandler);

private:
    awaitable<void> connect(boost::asio::io_context &io_context, const std::string &ip, unsigned short port,
                            std::function<void(std::shared_ptr<CommandSender>)> connectionEstablishedHandler);

    std::shared_ptr<Endpoint> _endpoint;
    ResponseReceivedHandler &_responseHandler;
};

#endif
