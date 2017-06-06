#include "basic_cpp_runtime.h"

using namespace basic_cpp;

namespace  {

enum Event {
    Ping = 1;
    Pong = 2;
    Success = 3;
};

class MachinePING : public PMachine {
private:
    static const bool _DeferedSet[][];
    static const bool _IgnoredSet[][];
    PMachine* pongId = NULL;
};

class MachinePONG : public PMachine {
private:
    static const bool _DeferedSet[][];
    static const bool _IgnoredSet[][];
};

};