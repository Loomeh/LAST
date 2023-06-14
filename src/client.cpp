#include <iostream>
#include <cstring>
#include <arpa/inet.h>

#include <lua.hpp>

#include "client.h"
#include "lasprint.h"

using std::string;
using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::runtime_error;
using std::to_string;
using std::exception;
using std::cerr;

int sock;

void connectToServer(string ipAddress)
{
    // Creating a socket for communication.
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        throw runtime_error("Socket error: " + to_string(errno));
    }

    int port = 16834;

    // Setting up the server address sructure.
    struct sockaddr_in hint;
    hint.sin_family = AF_INET; // IPv4 protocol family.
    hint.sin_port = htons(port); // Converting port to network byte order.
    if (inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr) != 1)
    {
        throw runtime_error("Invalid IP address");
    }

    // Connecting to LiveSplit Server.
    int connectRes = connect(sock, (struct sockaddr*) &hint, sizeof(hint));
    if (connectRes == -1)
    {
        throw runtime_error("Couldn't connect: " + to_string(errno));
    }

    lasPrint("Server: Connected\n");
}

void setIpAddress()
{
    string ipAddress;
    cout << "What is your local IP address? (Leave blank for 127.0.0.1)\n";
    getline(cin, ipAddress);
    if (ipAddress.empty()) {
        ipAddress = "127.0.0.1";
    }
    try
    {
        connectToServer(ipAddress);
    }
    catch (const exception& e)
    {
        cerr << "\n\033[1;31m" << e.what() << endl << endl;
        throw;
    }
}

void sendLiveSplitCommand(const char* command)
{
    // Create a buffer to hold the modified command with "\r\n"
    const size_t bufferSize = strlen(command) + 4; // Add space for "\r\n"
    char* modifiedCommand = new char[bufferSize];
    snprintf(modifiedCommand, bufferSize, "%s\r\n", command);

    int sendRes = send(sock, modifiedCommand, strlen(modifiedCommand), 0);

    delete[] modifiedCommand; // Free the memory allocated for the modified command

    if (sendRes == -1)
    {
        throw runtime_error("Couldn't send " + string(command) + ": " + to_string(errno));
    }
}

int sendCommand(lua_State* L)
{
    try
    {
        sendLiveSplitCommand(lua_tostring(L, 1));
    }
    catch (const exception& e)
    {
        cerr << "\033[1;31m" << e.what() << endl << endl;
        throw;
    }

    return 0;
}