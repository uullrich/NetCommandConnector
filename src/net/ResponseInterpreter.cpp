#include "../../include/ResponseInterpreter.h"

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

#include "../../include/Response.h"

Response ResponseInterpreter::interpret(std::string &message) {
    Response response{
            .type = ResponseType::UNKNOWN
    };
    message.erase(std::remove(message.begin(), message.end(), '\n'), message.cend());

    StringVector split;
    boost::split(split, message, boost::is_any_of(" "));

    if (split.size() == 2) {
        if (split.front() == "GET-RESPONSE") {
            response.type = ResponseType::GET;
        } else if (split.front() == "GET-DATA-RESPONSE") {
            response.type = ResponseType::GET_DATA;
        } else if (split.front() == "SET-RESPONSE") {
            response.type = ResponseType::SET;
        } else if (split.front() == "SET-DATA-RESPONSE") {
            response.type = ResponseType::SET_DATA;
        } else if (split.front() == "ERROR-RESPONSE") {
            response.type = ResponseType::ERROR;
        }

        auto uri = split.back();
        split.clear();
        boost::split(split, uri, boost::is_any_of("/"));
        response.parts = split;

        split.clear();
        boost::split(split, response.parts.back(), boost::is_any_of("="));

        if (split.size() == 2) {
            response.parts.back() = split.front();
            response.value = split.back();
        }
    }

    return response;
}