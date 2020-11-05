//
// Created by Lone Kriss on 2020/10/31.
//

#include "Client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<iostream>
using namespace std;

inline void error_message(char* message){
    cout<<message<<'\n';
    exit(1);
}

int main(int arg,char *argv[]){

    int sock;  //定义套接字
    struct sockaddr_in serv_addr;
    char buffer[30];
    int str_len;

    if(arg!=3){
        cout<<argv[0]<<" server <IP> <port> ";
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if( sock == -1 ){
        error_message("worry sock!");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family =AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr) ) == -1){
        error_message("connect sock!");
    }
    int i  = 0;

    while(i< 10){
        str_len = read(sock, buffer, sizeof(buffer));
        if(str_len == -1){
            error_message("read error!");
        }
        ++i;
        cout<<buffer;
    }

    close(sock);
    return 0;
}