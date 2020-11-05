//
// Created by Lone Kriss on 2020/11/4.
//

#include <cstdio>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<iostream>
#include<memory.h>

using namespace std;

inline void error_handle(const string& message){
    cout<<message<<'\n';
    exit(1);
}

int main(int argc,char* argv[]){

    if(argc!=2){
        cout<<argv[0]<<" <PORT>\n";
        exit(1);
    }


    int serv_sock = 0, clnt_sock = 0, clnt_sock1= 0;
    struct sockaddr_in serv_addr,clnt_addr;
    char buffer[1024];
    int str_len = 0;

    struct caculator{
        char numofcacul;
        char oper;
    };

   struct caculator cacula;

    serv_sock = socket(PF_INET,SOCK_STREAM,0);

    memset(&cacula,0,sizeof(cacula));
    memset(buffer,0, 1024);

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    socklen_t clnt_addr_len;



    if(serv_sock == -1){
        error_handle("socket error!\n");
        exit(1);
    }


    if(::bind(serv_sock,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1){
        error_handle("bind error!\n");
        exit(1);
    }
    if(listen(serv_sock, 5) == -1){
        error_handle("listen error!\n");
        exit(1);
    }

//----------------
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_len);
    if(clnt_sock == -1){
        error_handle("accpet error!\n");
        exit(0);
    }    else{
cout<<"conneted \n";
    }

    str_len = read(clnt_sock,&buffer, 1024);
    if(str_len == -1){
        error_handle("read error!\n");
        exit(1);
    }
    cacula.numofcacul = buffer[0];
    cacula.oper = buffer[1];

    int* dight = ((int*)&buffer[4]);

    int i;
    for(i=1;i<atoi(&cacula.numofcacul);++i){

        switch(cacula.oper){
            case '+':
                *dight += *(int*)&buffer[4+i*4];
                break;
            case '-':
                *dight -= *(int*)&buffer[4+i*4];
                break;
            case '*':
                *dight*=*(int*)&buffer[4+i*4];
                break;
            case '/':
                *dight/=*(int*)&buffer[4+i*4];
                break;
        }
    }

    cout<<*dight<<'\n';

    write(clnt_sock,(char*)dight,sizeof(dight));

    close(serv_sock);
    close(clnt_sock);
    return 0;
}