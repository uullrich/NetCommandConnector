#include "../../include/CommandConnector.h"

#include <boost/asio/io_context.hpp>
#include <iostream>
#include <utility>

#include "../../include/CommandSender.h"
#include "../../include/ResponseReceivedHandler.h"

CommandConnector::CommandConnector(const std::string &ip, unsigned short port,
                                   ResponseReceivedHandler &responseHandler,
                                   std::function<void(std::shared_ptr<CommandSender>)> connectionEstablishedHandler) :
        _responseHandler(responseHandler) {
    try {
        boost::asio::io_context io_context(1);
        co_spawn(io_context, connect(io_context, ip, port, std::move(connectionEstablishedHandler)), detached);
        io_context.run();
    } catch (std::exception &e) {
        std::printf("Exception: %s\n", e.what());
    }
}

awaitable<void>
CommandConnector::connect(boost::asio::io_context &io_context, const std::string &ip, unsigned short port,
                          std::function<void(std::shared_ptr<CommandSender>)> connectionEstablishedHandler) {
    std::cout << "Connect" << std::endl;
    _endpoint = std::make_shared<Endpoint>(io_context, ip, port, _responseHandler, connectionEstablishedHandler);
    std::cout << "Endpoint created" << std::endl;
    co_return;
}