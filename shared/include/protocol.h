#pragma once

#include "string.h"

namespace protocol
{
    // TODO - remove constexpr?
    const static constexpr char* const SERVER_CONNECT_ACCEPT = "159 ACK";
    static constexpr int SERVER_CONNECT_ACCEPT_LEN = strlen(SERVER_CONNECT_ACCEPT);
    
    const static constexpr char* const SERVER_CONNECT_REJECT = "000 NAK";
    static constexpr int SERVER_CONNECT_REJECT_LEN = strlen(SERVER_CONNECT_REJECT);

    const static constexpr char* const CLIENT_CONNECT_REQUEST = "459 CONN";
    static constexpr int CLIENT_CONNECT_REQUEST_LEN = strlen(CLIENT_CONNECT_REQUEST);

    const static constexpr char* const CLIENT_DISCONNECT_REQUEST = "444 DISCONN";
    static constexpr int CLIENT_DISCONNECT_REQUEST_LEN = strlen(CLIENT_DISCONNECT_REQUEST);

    const static constexpr char* const CLIENT_INPUT_HEADER = "8";
    static constexpr int CLIENT_INPUT_HEADER_LEN = strlen(CLIENT_INPUT_HEADER);

    const static constexpr char* const SERVER_DISCONNECT_REQUEST_ACK = "201 OK";
    static constexpr int SERVER_DISCONNECT_REQUEST_ACK_LEN = strlen(SERVER_DISCONNECT_REQUEST_ACK);

    const static constexpr char* const SERVER_DISCONNECT_TIMEOUT = "200 TIMEOUT";
    static constexpr int SERVER_DISCONNECT_TIMEOUT_LEN = strlen(SERVER_DISCONNECT_TIMEOUT);

    const static char SERVER_STATE_HEADER = 'S';
}

