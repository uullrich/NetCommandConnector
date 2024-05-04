#include "../../include/CommandSender.h"
#include <iostream>
#include <utility>
#include "../../include/Endpoint.h"

CommandSender::CommandSender(std::shared_ptr<Endpoint> endpoint)
        : _endpoint(std::move(endpoint)) {}

void CommandSender::get(const std::string &path) {
    std::stringstream command;
    command << "GET " << path << std::endl;
    _endpoint->send(command.str());
}

void CommandSender::getData(const std::string &path) {
    std::stringstream command;
    command << "GET-DATA " << path << std::endl;
    _endpoint->send(command.str());
}

void CommandSender::set(const std::string &path, const std::string &value) {
    std::stringstream command;
    command << "SET " << path << "=" << value << std::endl;
    _endpoint->send(command.str());
}

void CommandSender::setData(const std::string &path, const std::vector<unsigned char> &data) {
    std::stringstream commandStream;
    commandStream << "SET-DATA " << path << "=" << data.size() << std::endl;
    auto command = commandStream.str();

    _endpoint->send(command);
    _endpoint->send(data);
}

void CommandSender::setData(const std::string &path, const std::string &data) {
    std::vector<unsigned char> rawData(data.begin(), data.end());
    setData(path, rawData);
}