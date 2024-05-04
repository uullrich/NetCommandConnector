#ifndef RESPONSE_EXECUTION_INTERFACE_H
#define RESPONSE_EXECUTION_INTERFACE_H

#include <functional>

#include "../src/util/StringVector.h"

struct Response;

struct ResponseExecutionInterface {
    StringVector parts;
    std::function<void(Response &response)> setResponseMethod;
    std::function<void(Response &response)> getResponseMethod;
    std::function<void(Response &response)> setDataResponseMethod;
    std::function<void(Response &response)> getDataResponseMethod;
    std::function<void(Response &response)> errorResponseMethod;
};
#endif

