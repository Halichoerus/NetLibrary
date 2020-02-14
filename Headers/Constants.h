#ifndef CTP_CONSTANTS_H
#define CTP_CONSTANTS_H
namespace netlib {
    const int MAX_PACKET_SIZE = 100;
    const float PING_FREQUENCY = 1000;

    // MessageType is for internal use only
    // Every network message will start with a MessageType enum which defines how the rest of the message should be read.
    enum class MessageType : char {
        SINGLE_USER_MESSAGE, // A single packet message from the user - char... (data)
        MULTI_USER_MESSAGE, // A multi packet message from the user - uint(packetID) - uint(packetNum) -> uint(totalPackets) -> char...(data)
        SET_CLIENT_UID, // An internal message from the server to set the clients UID - uint(client uid)
        PING_REQUEST, // A 1 byte type only packet that needs to respond with a PING_RESPONSE packet immediately
        PING_RESPONSE, // A 1 byte type only packet that signifies the response of a PING_REQUEST

        // Events a client can send to the server
        REQUEST_NEW_LOBBY, // Signals for the server to create a new lobby and add this client to it - uint(blank) -> int(roomSize) -> uint(size of name) -> char...(lobby name)
        REMOVE_FROM_LOBBY, // Tells the server to remove the given client from its current lobby - uint(clientUID)
        JOIN_LOBBY, // Tells the server to add this client to the chosen lobby - uint(lobby uid)

        // Events the server can send to clients
        SET_ACTIVE_LOBBY, // Tells the client to set its active lobby - uint(lobbyUID)
        NEW_LOBBY_CLIENT, // Tells the client that a new client has joined a lobby - uint(lobby uid) -> uint(new client uid) -> uint(size of name) ->  char...(client name)
        LOBBY_CLIENT_LEFT, // Tells the client that another client has left a lobby - uint(lobby uid) -> uint(left client uid)
        UPDATE_PEER_CONNECTION_INFO, // Tells the client to update its current lobby peerInfo connection values - int(infoCount) -> float(ping) -- repeat per infoCount
        SET_CLIENT_READY, // Tells the client that a client has set been set to active/inactive - uint(client uid) -> bool(isReady)
        ADD_NEW_LOBBY, // Tells the client there is a new lobby open - uint(lobby uid) -> int(roomSize) -> uint(size of name) ->  char...(lobby name)
        REMOVE_LOBBY, // Tells the client to close a lobby - uint(lobby uid)


    };
}
#endif //CTP_CONSTANTS_H
