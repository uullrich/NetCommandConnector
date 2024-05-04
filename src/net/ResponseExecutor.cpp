#include "../../include/ResponseExecutor.h"

#include "../../include/Response.h"
#include "../../include/ResponseExecutionInterface.h"

void
ResponseExecutor::doResponseExecution(Response &response, std::vector<ResponseExecutionInterface> &execInterfaces) {
    auto commandPartsCount = response.parts.size();
    for (int i = 0; i < execInterfaces.size(); i++) {
        auto &execInterface = execInterfaces[i];
        bool match = true;
        if (commandPartsCount >= execInterface.parts.size()) {
            for (int j = 0; j < execInterface.parts.size(); j++) {
                if (execInterface.parts[j] != "*" && execInterface.parts[j] != response.parts[j]) {
                    match = false;
                    break;
                }
            }
        }

        if (match) {
            if (execInterface.setResponseMethod && response.type == ResponseType::SET) {
                execInterface.setResponseMethod(response);
            } else if (execInterface.getResponseMethod && response.type == ResponseType::GET) {
                execInterface.getResponseMethod(response);
            } else if (execInterface.setDataResponseMethod && response.type == ResponseType::SET_DATA) {
                execInterface.setDataResponseMethod(response);
            } else if (execInterface.getDataResponseMethod && response.type == ResponseType::GET_DATA) {
                execInterface.getDataResponseMethod(response);
            } else if (response.type == ResponseType::ERROR) {
                execInterface.errorResponseMethod(response);
            } else {
                //Command not supported
                //ToDo: Log output
            };
            break;
        }
    }
}
