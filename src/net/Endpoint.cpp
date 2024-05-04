#include "../../include/Endpoint.h"

#include <iostream>
#include <boost/asio/redirect_error.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio.hpp>
#include <boost/asio/read_until.hpp>
#include <utility>

#include "../../include/CommandSender.h"
#include "../../include/ResponseInterpreter.h"
#include "../../include/Response.h"
#include "../../include/ResponseReceivedHandler.h"

Endpoint::Endpoint(boost::asio::io_context &io_context, const std::string &ip, unsigned short port,
                   ResponseReceivedHandler &responseHandler,
                   std::function<void(std::shared_ptr<CommandSender>)> connectionEstablishedHandler) :
        _ip(ip),
        _port(port),
        _socket(io_context),
        _resolver(io_context),
        _timer(io_context),
        _responseHandler(responseHandler),
        _connectionEstablishedHandler(std::move(connectionEstablishedHandler)) {
    _timer.expires_at(std::chrono::steady_clock::time_point::max());
    co_spawn(io_context, initConnection(), detached);
}

void Endpoint::send(const std::string &data) {
    _sendBuffer.emplace_back(data.begin(), data.end());
    _timer.cancel_one();
}

void Endpoint::send(const std::vector<unsigned char> &data) {
    _sendBuffer.emplace_back(data);
    _timer.cancel_one();
}

awaitable<void> Endpoint::initConnection() {
    try {
        auto resolved = co_await resolve();
        co_await connect(resolved);

        co_spawn(_socket.get_executor(),
                 [self = shared_from_this()] { return self->onSendHandler(); }, detached);

        co_spawn(
                _socket.get_executor(),
                [self = shared_from_this()] { return self->onReceiveHandler(); },
                detached);

        auto commandSender = std::make_shared<CommandSender>(shared_from_this());
        _connectionEstablishedHandler(commandSender);
    } catch (std::exception &e) {
        std::cerr << "failed to initialize the connection : " << e.what() << std::endl;
    }
}

awaitable<const tcp::resolver::iterator> Endpoint::resolve() {
    const auto iterator = co_await _resolver.async_resolve(_ip, std::to_string(_port), use_awaitable);
    co_return iterator;
}

awaitable<void> Endpoint::connect(const tcp::resolver::iterator &iterator) {
    auto result = co_await boost::asio::async_connect(_socket, iterator, use_awaitable);
    std::cout << "Connection to " << result->host_name() << " successful" << std::endl;
}

awaitable<void> Endpoint::onSendHandler() {
    try {
        while (_socket.is_open()) {
            if (_sendBuffer.empty()) {
                boost::system::error_code ec;
                co_await _timer.async_wait(redirect_error(use_awaitable, ec));
            } else {
                auto sendData = _sendBuffer.front();
                std::string debug(sendData.begin(), sendData.end());
                std::cout << "SendData:" << debug << std::endl;
                co_await boost::asio::async_write(_socket,
                                                  boost::asio::buffer(sendData), use_awaitable);
                _sendBuffer.pop_front();
            }
        }
    }
    catch (std::exception &) {
        disconnect();
    }
}

awaitable<void> Endpoint::onReceiveHandler() {
    try {

        while (true) {
            std::size_t delimiterPosition = co_await boost::asio::async_read_until(_socket,
                                                                                   boost::asio::dynamic_buffer(
                                                                                           _receiveBuffer),
                                                                                   "\n",
                                                                                   use_awaitable);

            std::string receivedMessage(_receiveBuffer.begin(),
                                        _receiveBuffer.begin() + static_cast<long>(delimiterPosition));

            _receiveBuffer.erase(_receiveBuffer.begin(), _receiveBuffer.begin() + static_cast<long>(delimiterPosition));

            auto response = ResponseInterpreter::interpret(receivedMessage);
            response.endpoint = shared_from_this();

            if (response.type == ResponseType::GET_DATA || response.type == ResponseType::ERROR) {
                int expectedBytes = std::stoi(response.value);
                if (_receiveBuffer.size() >= expectedBytes) {
                    response.data.insert(response.data.end(), _receiveBuffer.begin(),
                                         _receiveBuffer.begin() + expectedBytes);
                    _receiveBuffer.erase(_receiveBuffer.begin(), _receiveBuffer.begin() + expectedBytes);
                } else {
                    int missingBytes = expectedBytes - _receiveBuffer.size();
                    response.data.insert(response.data.end(), _receiveBuffer.begin(), _receiveBuffer.end());
                    _receiveBuffer.erase(_receiveBuffer.begin(), _receiveBuffer.end());

                    co_await boost::asio::async_read(_socket,
                                                     boost::asio::dynamic_buffer(_receiveBuffer),
                                                     boost::asio::transfer_exactly(missingBytes),
                                                     use_awaitable);

                    response.data.insert(response.data.end(), _receiveBuffer.begin(),
                                         _receiveBuffer.begin() + missingBytes);
                    _receiveBuffer.erase(_receiveBuffer.begin(), _receiveBuffer.begin() + missingBytes);
                }
            }

            if (response.type != ResponseType::UNKNOWN) {
                _responseHandler.onResponseReceived(response);
            }
        }
    }
    catch (std::exception &exception) {
        std::cout << "Error: " << exception.what() << std::endl;
        disconnect();
    }
}

void Endpoint::disconnect() {
    _socket.close();
    _timer.cancel();
}