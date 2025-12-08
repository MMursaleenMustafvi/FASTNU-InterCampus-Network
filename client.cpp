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

#define SERVER_IP "127.0.0.1"
#define TCP_PORT 8080
#define UDP_PORT 8081

// --- COLORS ---
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"
#define BG_RED  "\033[41m"

char campus_name[50];
string campus_pass;
mutex printMutex;

string getCredential(const char *campus, const char *pass)
{
    if (strcmp(campus, "Lahore") == 0 && strcmp(pass, "NU-LHR-123") == 0) return "Campus:Lahore,Pass:NU-LHR-123";
    if (strcmp(campus, "Karachi") == 0 && strcmp(pass, "NU-KHI-123") == 0) return "Campus:Karachi,Pass:NU-KHI-123";
    if (strcmp(campus, "Peshawar") == 0 && strcmp(pass, "NU-PSH-123") == 0) return "Campus:Peshawar,Pass:NU-PSH-123";
    if (strcmp(campus, "Islamabad") == 0 && strcmp(pass, "NU-ISB-123") == 0) return "Campus:Islamabad,Pass:NU-ISB-123";
    if (strcmp(campus, "CFD") == 0 && strcmp(pass, "NU-CFD-123") == 0) return "Campus:CFD,Pass:NU-CFD-123";
    if (strcmp(campus, "Multan") == 0 && strcmp(pass, "NU-MLT-123") == 0) return "Campus:Multan,Pass:NU-MLT-123";
    return "";
}

void printPrompt()
{
    cout << YELLOW << "\rYou > " << RESET;
    cout.flush();
}

void receiveMessages(int tcpSocket)
{
    char buffer[4096];

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(tcpSocket, buffer, sizeof(buffer), 0);

        if (bytes <= 0)
        {
            printMutex.lock();
            cout << RED << "\n[SYSTEM] Connection lost." << RESET << endl;
            printMutex.unlock();
            exit(0);
        }

        string msg(buffer);
        size_t pos = msg.find(':');
        string sender = (pos != string::npos) ? msg.substr(0, pos) : "SYSTEM";
        string content = (pos != string::npos) ? msg.substr(pos + 1) : msg;

        printMutex.lock();
        cout << "\r\033[K";

        if (sender == "SERVER")
        {
            cout << BG_RED << BOLD << "[SERVER BROADCAST]" << RESET << " " << RED << content << RESET << endl;
        }
        else if (sender == "SYSTEM")
        {
            cout << RED << "[!]" << RESET << " " << content << endl;
        }
        else
        {
            cout << CYAN << BOLD << "[" << sender << "]" << RESET << ": " << content << endl;
        }

        printPrompt();
        printMutex.unlock();
    }
}

void udpHeartbeat()
{
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(UDP_PORT);
    addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    while (true)
    {
        string msg = string(campus_name) + " ONLINE";

        sendto(udpSocket, msg.c_str(), msg.size(), 0, (sockaddr*)&addr, sizeof(addr));

        sleep(5);
    }
}

void displayBanner()
{
    system("clear");
    cout << BOLD << CYAN;
    cout << "********************************************" << endl;
    cout << "* FAST NU INTER-CAMPUS LINK               *" << endl;
    cout << "********************************************" << RESET << endl;
}

int main()
{
    displayBanner();

    cout << BOLD << "Enter Campus (e.g., Lahore): " << RESET;
    cin >> campus_name;
    cin.ignore();

    cout << BOLD << "Enter Password: " << RESET;
    getline(cin, campus_pass);

    string credential = getCredential(campus_name, campus_pass.c_str());

    if (credential == "")
    {
        cout << RED << "Invalid credentials." << RESET << endl;
        return 0;
    }

    int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(TCP_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(tcpSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        cout << RED << "Failed to connect to server." << RESET << endl;
        return 0;
    }

    send(tcpSocket, credential.c_str(), credential.size(), 0);

    char resp[128];
    memset(resp, 0, sizeof(resp));
    recv(tcpSocket, resp, sizeof(resp), 0);

    if (strcmp(resp, "DUPLICATE_LOGIN") == 0)
    {
        cout << RED << "Campus already online." << RESET << endl;
        close(tcpSocket);
        return 0;
    }

    cout << GREEN << "Login Successful!" << RESET << endl;

    thread(receiveMessages, tcpSocket).detach();
    thread(udpHeartbeat).detach();

    while (true)
    {
        string input;

        printMutex.lock();
        printPrompt();
        printMutex.unlock();

        getline(cin, input);

        if (input == "exit")
        {
            send(tcpSocket, "Exit", 4, 0);
            close(tcpSocket);
            return 0;
        }

        if (input.find(':') == string::npos)
        {
            printMutex.lock();
            cout << RED << "[!] Invalid format. Use Target:Message" << RESET << endl;
            printMutex.unlock();
            continue;
        }

        send(tcpSocket, input.c_str(), input.size(), 0);

        printMutex.lock();
        cout << "\033[A\033[K";
        cout << GREEN << "You -> " << input << RESET << endl;
        printMutex.unlock();
    }

    return 0;
}

