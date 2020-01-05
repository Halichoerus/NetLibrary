#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <thread>

#include "Client.h"

Client::~Client()
{
    Stop();
    deleteSafeguard->lock();
    deleteSafeguard->unlock();
}

void Client::Stop()
{
    running = false;
    close(sock);
}

bool Client::Start(const std::string &ipv4)
{
    deleteSafeguard = new std::mutex();
    std::cout << "Initializing Client..." << std::endl;
    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }

    // Create hint structure
    int port = 54000;

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipv4.c_str(), &hint.sin_addr);

    // Connect to the server
    int conResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if(conResult == -1)
    {
        std::cerr << "Failed to connect to the server" << std::endl;
        return false;
    }

    std::cout << "Client successfully initilized!" << std::endl;

    std::thread tr(&Client::ProcessNetworkEvents, this);
    tr.detach();

    return true;
}

void Client::SendMessage(const char *data, int dataLength)
{
    // If there is no data dont send it
    if(dataLength > 0)
    {
        int sendResult = send(sock, data, dataLength, 0);
        if (sendResult == -1)
        {
            std::cerr << "Error in sending data" << std::endl;
        }
    }
}

void Client::ProcessNetworkEvents()
{
    deleteSafeguard->lock();
    running = true;
    // Loop to send and receive data
    char buf[4096];

    while(running)
    {
        // Wait for response
        memset(buf,0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);

        if (bytesReceived > 0)
        {
            auto packet = new DataPacket();
            packet->data = new char[bytesReceived];
            memcpy(packet->data,buf, bytesReceived);
            packet->dataLength = bytesReceived;
            processPacket(packet);
            // Echo response to console
            //std::cout << std::string(buf, 0, bytesReceived) << std::endl;
        }
    }

    deleteSafeguard->unlock();
}