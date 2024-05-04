#ifndef RESPONSE_H
#define RESPONSE_H

#include <vector>
#include <string>

#include "Endpoint.h"
#include "../src/util/StringVector.h"
#include "../src/util/Buffer.h"

enum class ResponseType {
    UNKNOWN,
    GET,
    GET_DATA,
    SET,
    SET_DATA,
    ERROR
};

struct Response {
    ResponseType type;
    StringVector parts;
    std::string value;
    ReceiveBuffer data;
    EndpointPtr endpoint;
};

#endif
