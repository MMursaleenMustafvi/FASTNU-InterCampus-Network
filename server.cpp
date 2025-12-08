#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <thread>
#include <mutex>

using namespace std;

#define TCP_PORT 8080
#define UDP_PORT 8081
#define MAX_CLIENTS 20

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"
#define BOLD "\033[1m"

struct ClientInfo
{
    int socket;
    string campus;
};

ClientInfo clients[MAX_CLIENTS];
int clientCount = 0;
mutex mtx;

string validateCampus(const string &cred)
{
    if (cred == "Campus:Lahore,Pass:NU-LHR-123") return "Lahore";
    if (cred == "Campus:Karachi,Pass:NU-KHI-123") return "Karachi";
    if (cred == "Campus:Peshawar,Pass:NU-PSH-123") return "Peshawar";
    if (cred == "Campus:Islamabad,Pass:NU-ISB-123") return "Islamabad";
    if (cred == "Campus:CFD,Pass:NU-CFD-123") return "CFD";
    if (cred == "Campus:Multan,Pass:NU-MLT-123") return "Multan";
    return "";
}

bool isCampusOnline(const string &campus)
{
    for (int i = 0; i < clientCount; i++)
    {
        if (clients[i].campus == campus) return true;
    }
    return false;
}

void removeClient(int socket)
{
    mtx.lock();

    for (int i = 0; i < clientCount; i++)
    {
        if (clients[i].socket == socket)
        {
            cout << RED << "[DISCONNECT] " << clients[i].campus << " offline." << RESET << endl;
            clients[i] = clients[clientCount - 1];
            clientCount--;
            break;
        }
    }

    mtx.unlock();
    close(socket);
}

void broadcastMessage(const string &message)
{
    mtx.lock();

    string formatted = "SERVER:" + message;

    for (int i = 0; i < clientCount; i++)
    {
        send(clients[i].socket, formatted.c_str(), formatted.size(), 0);
    }

    mtx.unlock();

    cout << MAGENTA << "[BROADCAST SENT] " << message << RESET << endl;
}

void handleClient(int clientSocket)
{
    char buffer[4096];

    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytes <= 0)
    {
        close(clientSocket);
        return;
    }

    string cred(buffer, bytes);
    string campus = validateCampus(cred);

    if (campus == "")
    {
        close(clientSocket);
        return;
    }

    mtx.lock();

    if (isCampusOnline(campus))
    {
        send(clientSocket, "DUPLICATE_LOGIN", 16, 0);
        mtx.unlock();
        close(clientSocket);
        return;
    }

    send(clientSocket, "OK", 2, 0);

    clients[clientCount].socket = clientSocket;
    clients[clientCount].campus = campus;
    clientCount++;

    mtx.unlock();

    cout << CYAN << "[CONNECTED] " << campus << RESET << endl;

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int n = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (n <= 0)
        {
            removeClient(clientSocket);
            return;
        }

        string msg(buffer, n);

        if (msg == "Exit")
        {
            removeClient(clientSocket);
            return;
        }

        int pos = msg.find(':');
        if (pos == string::npos) continue;

        string target = msg.substr(0, pos);
        string text = msg.substr(pos + 1);

        bool found = false;

        mtx.lock();

        for (int i = 0; i < clientCount; i++)
        {
            if (clients[i].campus == target)
            {
                string forwardMsg = campus + ":" + text;
                send(clients[i].socket, forwardMsg.c_str(), forwardMsg.size(), 0);
                found = true;
                break;
            }
        }

        mtx.unlock();

        if (!found)
        {
            string notOnline = "SYSTEM:" + target + " is not online.";
            send(clientSocket, notOnline.c_str(), notOnline.size(), 0);
        }
        else
        {
            cout << GREEN << "[ROUTE]" << RESET << " " << campus << " -> " << target << endl;
        }
    }
}

void udpHeartbeatServer()
{
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(UDP_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(udpSocket, (sockaddr*)&addr, sizeof(addr));

    char buffer[2048];

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        recv(udpSocket, buffer, sizeof(buffer), 0);

        cout << CYAN << "[HEARTBEAT] " << RESET << buffer << endl;
    }
}

void acceptConnections()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(TCP_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 10);

    while (true)
    {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        thread(handleClient, clientSocket).detach();
    }
}

int main()
{
    system("clear");
    cout << BOLD << MAGENTA;
    cout << "========================================" << endl;
    cout << "        FAST-NU CENTRAL SERVER" << endl;
    cout << "========================================" << RESET << endl;

    thread(udpHeartbeatServer).detach();
    thread(acceptConnections).detach();

    string adminMsg;

    while (true)
    {
        getline(cin, adminMsg);

        if (!adminMsg.empty())
        {
            broadcastMessage(adminMsg);
        }
    }

    return 0;
}

