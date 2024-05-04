#ifndef RESPONSE_INTERPRETER_H
#define RESPONSE_INTERPRETER_H

#include <string>

struct Response;

class ResponseInterpreter {
public:
    static Response interpret(std::string &message);
};

#endif
