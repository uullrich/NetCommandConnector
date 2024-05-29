#ifndef RESPONSE_EXECUTION_INTERFACE_H
#define RESPONSE_EXECUTION_INTERFACE_H

#include <iostream>
#include <functional>
#include <vector>

#include "../src/util/StringVector.h"

struct Response;

typedef StringVector Path;

enum class NO_RESPONSE {
    GET,
    GET_DATA,
    SET,
    SET_DATA,
    ERROR
};

auto noCommandHandler = [](Response &) {};

struct ResponseExecutionInterface {
    StringVector parts;
    std::function<void(Response &response)> setResponseMethod;
    std::function<void(Response &response)> getResponseMethod;
    std::function<void(Response &response)> setDataResponseMethod;
    std::function<void(Response &response)> getDataResponseMethod;
    std::function<void(Response &response)> errorResponseMethod;

    explicit ResponseExecutionInterface(void *instance) : _instance(instance) {}

    ResponseExecutionInterface(StringVector parts,
                               std::function<void(Response &)> setResponseMethod,
                               std::function<void(Response &)> getResponseMethod,
                               std::function<void(Response &)> setDataResponseMethod,
                               std::function<void(Response &)> getDataResponseMethod,
                               std::function<void(Response &)> errorResponseMethod
    )
            : parts(std::move(parts)),
              setResponseMethod(std::move(setResponseMethod)),
              getResponseMethod(std::move(getResponseMethod)),
              setDataResponseMethod(std::move(setDataResponseMethod)),
              getDataResponseMethod(std::move(getDataResponseMethod)),
              errorResponseMethod(std::move(errorResponseMethod)),
              _instance(nullptr) {}

    //Fluent interface for setting parts
    ResponseExecutionInterface &operator<<(const StringVector &p) {
        parts = p;
        return *this;
    }

    //Helper function to convert member function pointer to lambda
    template<typename T>
    std::function<void(Response &)> memberFunctionToLambda(void (T::*memFunc)(Response &)) {
        auto *inst = static_cast<T *>(_instance);
        return [memFunc, inst](Response &cmd) { (inst->*memFunc)(cmd); };
    }

    //Overload for setting member function pointers
    template<typename T>
    ResponseExecutionInterface &operator<<(void (T::*memFunc)(Response &)) {
        if (!_instance) {
            throw std::logic_error("Instance pointer is not set.");
        }

        if (!setResponseMethod) {
            setResponseMethod = memberFunctionToLambda(memFunc);
        } else if (!getResponseMethod) {
            getResponseMethod = memberFunctionToLambda(memFunc);
        } else if (!setDataResponseMethod) {
            setDataResponseMethod = memberFunctionToLambda(memFunc);
        } else if (!getDataResponseMethod) {
            getDataResponseMethod = memberFunctionToLambda(memFunc);
        } else if (!errorResponseMethod) {
            errorResponseMethod = memberFunctionToLambda(memFunc);
        } else {
            throw std::runtime_error("All method slots are already filled.");
        }
        return *this;
    }

    //Fluent interface for setting no response handlers
    ResponseExecutionInterface &operator<<(NO_RESPONSE noResponse) {

        switch (noResponse) {
            case NO_RESPONSE::SET:
                setResponseMethod = noCommandHandler;
                break;
            case NO_RESPONSE::GET:
                getResponseMethod = noCommandHandler;
                break;
            case NO_RESPONSE::SET_DATA:
                setDataResponseMethod = noCommandHandler;
                break;
            case NO_RESPONSE::GET_DATA:
                getDataResponseMethod = noCommandHandler;
                break;
            case NO_RESPONSE::ERROR:
                errorResponseMethod = noCommandHandler;
                break;
            default:
                throw std::runtime_error("All method slots are already filled.");
        }

        return *this;
    }

    friend std::vector<ResponseExecutionInterface> &
    operator<<(std::vector<ResponseExecutionInterface> &vec, const ResponseExecutionInterface &cmdExec);

private:
    void *_instance;
};

#endif

