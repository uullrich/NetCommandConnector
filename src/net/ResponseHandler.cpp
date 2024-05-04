#include "../../include/ResponseHandler.h"

#include "../../include/Response.h"
#include "../../include/ResponseExecutor.h"

ResponseHandler::ResponseHandler(std::vector<ResponseExecutionInterface> &responseExecutionInterface)
        : _responseExecutionInterface(responseExecutionInterface) {}

void ResponseHandler::onResponseReceived(Response &response) {
    ResponseExecutor::doResponseExecution(response, _responseExecutionInterface);
}