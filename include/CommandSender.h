#ifndef COMMAND_SENDER_H
#define COMMAND_SENDER_H

#include <string>

class Endpoint;

class CommandSender {
public:
    explicit CommandSender(std::shared_ptr<Endpoint> endpoint);

    void get(const std::string &path);

    void getData(const std::string &path);

    void set(const std::string &path, const std::string &value);

    void setData(const std::string &path, const std::vector<unsigned char> &data);

    void setData(const std::string &path, const std::string &data);

private:
    std::shared_ptr<Endpoint> _endpoint;
};

#endif
