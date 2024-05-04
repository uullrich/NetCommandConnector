#ifndef RESPONSE_EXECUTOR_H
#define RESPONSE_EXECUTOR_H

#include <vector>

struct ResponseExecutionInterface;
struct Response;

class ResponseExecutor {
public:
    static void doResponseExecution(Response &response, std::vector<ResponseExecutionInterface> &execInterfaces);
};

#endif
