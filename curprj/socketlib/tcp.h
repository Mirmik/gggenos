#include <iostream>   
#include <stdio.h>
#include <string.h>

#include <netinet/tcp.h>


#include <sys/socket.h>
#include <arpa/inet.h> 
#include <netdb.h> 
#include <unistd.h>
#include "genos/sigslot/delegate.h"
#include "genos/container/dlist.h"

using namespace std;

class Socket
{
public:
    int sock;

    int open(int domain, int type, int protocol)
    {
        if (sock == -1) { close(); };

        sock = socket(domain , type, protocol);
        if (sock == -1)
        {
            perror("Could not create socket");
        }
    };    

    int close()
    {
        ::close(sock);
    };

    int get()
    {
        return sock;
    };

    int nodelay()
    {
        int optval = 1;
        setsockopt(sock, IPPROTO_TCP, TCP_NODELAY,  &optval, sizeof(optval));
    };

    operator bool() const
    {
        return sock != -1;
    };

    Socket() : sock(-1) {};
};

class TcpSocket
{

};

class TcpServer
{
private:
    Socket sock;
    sockaddr_in addr;
    
    int port;
    delegate<void, void*> connection_handler;
     
public:
    TcpServer() {};

    void begin()
    {
        sock.open(AF_INET, SOCK_STREAM, 0);
    };

    void nodelay()
    {
        sock.nodelay();
    };

    void end()
    {
        
    };

    int bind(int port)
    {
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        if (
            ::bind(sock.get(), (struct sockaddr*) &addr, sizeof(addr)) == -1
            )
        {
            perror("bind error");
            return -1; 
        };
        return 0;
    }

    int listen(int backlog)
    {
        if (
            ::listen(sock.get(), backlog) == -1
            )
        {
            perror("listen error");
            return -1; 
        };
        return 0;
    }

    struct Client
    {
        TcpServer* serv;
        int socket;     
    };

    #include "pthread.h"
    void cycling_accept( void*(*request_handler)(void*) )
    {
        int client, c;
        sockaddr_in client_addr;
        c = sizeof(sockaddr_in);
        pthread_t handler_thread;

        while
        (
            client = ::accept(
                sock.get(), 
                (sockaddr*)&client_addr, 
                (socklen_t*)&c
                )
        )
        {
            /*if(client_addr == -1) 
            {
                if(errno == EINTR) {
                    continue;
                }
                else {
                    perror("accept failed");
                    exit(1);
                }
            }*/

            TcpServer::Client* cl = new TcpServer::Client;
            cl -> serv = this;
            cl -> socket = client;

            if(
                pthread_create(
                    &handler_thread, 
                    NULL, 
                    request_handler, 
                    cl
                    ) 
                    != 0
            ) 
            {
                perror("pthread_create failed");
            };
        };

    }
};
