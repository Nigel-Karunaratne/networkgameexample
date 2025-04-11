#pragma once

#include "string.h"

namespace protocol
{
    // TODO - remove constexpr?
    const static constexpr char* const SERVER_CONNECT_ACCEPT = "459 ACK";
    static constexpr int SERVER_CONNECT_ACCEPT_LEN = strlen(SERVER_CONNECT_ACCEPT);
    
    const static constexpr char* const SERVER_CONNECT_REJECT = "000 NAK";
    static constexpr int SERVER_CONNECT_REJECT_LEN = strlen(SERVER_CONNECT_REJECT);

    const static constexpr char* const CLIENT_CONNECT_REQUEST = "459 CONN";
}

