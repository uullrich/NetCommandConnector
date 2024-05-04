#ifndef RESPONSE_HANDLER_H
#define RESPONSE_HANDLER_H

#include "ResponseReceivedHandler.h"

struct Response;
struct ResponseExecutionInterface;

class ResponseHandler : public ResponseReceivedHandler {
public:
    explicit ResponseHandler(std::vector<ResponseExecutionInterface> &responseExecutionInterface);

    void onResponseReceived(Response &response) override;

private:
    std::vector<ResponseExecutionInterface> &_responseExecutionInterface;
};

#endif