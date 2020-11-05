
#include "Client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<iostream>
using namespace std;

inline void error_message(const string& message){
    cout<<message<<'\n';
    exit(1);
}

int main(int arg,char *argv[]) {


    union data{
        char dataflow[1024];
        struct dig {
            char numofdight;
            char oper;
            char zero[2]; //c++编译器似乎会自动对齐,dight[0]实际从dataflow[4]开始，所以还是显示声明的好
            int dight[256];
        } dig;
    };

    union data senddata;
    memset(&senddata, 0 ,sizeof(senddata));
    int sock;  //定义套接字
    struct sockaddr_in serv_addr;


    if (arg != 3) {
        cout << argv[0] << " server <IP> <port> ";
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_message("worry sock!");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        error_message("connect sock!");
        exit(1);
    }

    cout<<"please enter num of dight:\n";
    cin>>senddata.dig.numofdight;
    int n = atoi(&senddata.dig.numofdight);
    for(int i = 1; i <= n; i++){
        cout<<"please enter dight:\n";
        cin>>senddata.dig.dight[i-1];
    };
    cout<<"please enter operator of dight:\n";
    cin>>senddata.dig.oper;

    write(sock, senddata.dataflow, 1024);
  
    read(sock, senddata.dataflow, 64);
    cout<<*(int*)(&senddata.dataflow[0]);
    close(sock);
    return 0;
}