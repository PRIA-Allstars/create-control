#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

#include "create.h"

static volatile int stopServer = 0;

#define SERVER_PORT 12345
#define SERVER_IP 10.0.0.1

static void handler(int dummy)
{
    "SIGINT caught, shutting down."
    stopServer = 1;
    shutdown(server_socket,2);
}

int main()
{
    // register SIGINT handler
    signal(SIGINT, handler);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adressinfo;
    unsigned short int portnumber = SERVER_PORT;
    char ip_adress[13] = SERVER_IP;

    adressinfo.sin_family = AF_INET;
    adressinfo.sin_addr.s_addr = INADDR_ANY;
    adressinfo.sin_port = htons(portnumber);

    printf("server: binding socket to port %d on IP %s... ",portnumber,ip_adress);
    bind(server_socket, (struct sockaddr*)&adressinfo, sizeof(adressinfo));
    printf("done\n");

    printf("server: listening to socket\n");
    listen(server_socket, 3);

    int8_t msg[3];
    msg[2]='\0';

    int read_error = 1;
    uint8_t client_connected = 0;

    create_connect();
    int16_t spin = 0;
    int16_t drive = 0;

    while(!stopServer)
    {
        printf("server: waiting for connections... \n");
        int connfd = accept(server_socket, (struct sockaddr*)NULL, NULL);

        //get ip from client
        struct sockaddr_in addr;
        socklen_t addr_size = sizeof(struct sockaddr_in);
        getpeername(connfd, (struct sockaddr *)&addr, &addr_size);
        char clientip[20];
        strcpy(clientip, inet_ntoa(addr.sin_addr));

        printf("ManCtrlThread: client connected, IP: %s\n",clientip);
        client_connected = 1;

        struct timeval tv;
        gettimeofday(&tv,NULL);
        uint64_t time_old = tv.tv_sec*1000000 + tv.tv_usec;  //in us

        while(client_connected)
        {
            //wait for new message
            read_error = read(connfd,msg,5);
            if(read_error==-1)
            {
                printf("server: recv error! %s\n", strerror(errno));
            }
            else if(read_error==0)
            {
                printf("server: client disconnected!\n");
                client_connected = 0;
            }
            else
            {

                drive = msg[0];
                spin = 0;
                if(drive < 10 && drive > -10)
                {
                    drive = 0;
                }

                spin = msg[1];
                if(spin < 10 && spin > -10)
                {
                    spin = 0;
                }

                if(drive)
                {
                    create_drive_direct(drive, drive);
                }
                else if(spin)
                {
                    create_drive_direct(spin, -1 * spin);
                }
                else
                {
                    create_drive_direct(0, 0);
                }
                printf("forward: %i\n", [0]);
                printf("spin: %i\n", msg[1]);
            }
        }
    }
    create_disconnect();
    return 0;
}
