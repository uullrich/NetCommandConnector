#include "../../include/ResponseExecutionInterface.h"

std::vector<ResponseExecutionInterface>& operator<<(std::vector<ResponseExecutionInterface>& vec, const ResponseExecutionInterface& cmdExec) {
    vec.push_back(cmdExec);
    return vec;
}