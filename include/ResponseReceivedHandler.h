#ifndef RESPONSE_RECEIVED_HANDLER_H
#define RESPONSE_RECEIVED_HANDLER_H

#include <iostream>

struct Response;

class ResponseReceivedHandler {
public:
    virtual void onResponseReceived(Response &response) = 0;
};

#endif