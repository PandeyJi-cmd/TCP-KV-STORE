#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h> // for error
#include <unistd.h>
#include <arpa/inet.h> //for Networking
#include <sys/socket.h> // for Networking
#include <netinet/ip.h> // for Networking
void die(const char* msg){
    perror(msg);
    exit(EXIT_FAILURE);
}
int main(){
    int fd=socket(AF_INET,SOCK_STREAM,0); // IPv4 + TCP

    if(fd < 0){
        die("SOCKET IS NOT FORMED");
    }

    int val=1;
    setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));


    struct sockaddr_in addr={};
    addr.sin_family= AF_INET;
    addr.sin_port=htons(6379); // for port number .. in big endian
    addr.sin_addr.s_addr=htonl(0); //for ip in big endian most significant first network byte order

    int rv=bind(fd,(const struct sockaddr* )&addr,sizeof(addr));
    if(rv) { die("NOT BOUNDED");}
    int rv_listen = listen(fd,SOMAXCONN);
    printf("The server is Listening \n");
    while(true){
        struct sockaddr_in client={};
        socklen_t client_size = sizeof(client);
        int con_nfd=accept(fd, (struct sockaddr*)&client,&client_size);
        if(con_nfd < 0 ){
            continue;
        }
        printf("Connection Established \n");
        // now lets read some stuff 
        char buffer[1024]={};
        ssize_t bytes=read(con_nfd,buffer,sizeof(buffer)-1);
        if(bytes<0) continue;
        else{
            printf("Client said : %s \n ",buffer);
            const char *response="OK\n";
            write(con_nfd,response,strlen(response));
        }
        close(con_nfd);
        printf("Connection Closed\n");
    }
}

//tokenizer
//
