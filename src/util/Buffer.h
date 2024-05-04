#ifndef BUFFER_H
#define BUFFER_H

#include <deque>
#include <vector>

typedef std::deque<std::vector<unsigned char>> SendBuffer;
typedef std::vector<unsigned char> ReceiveBuffer;
#endif
