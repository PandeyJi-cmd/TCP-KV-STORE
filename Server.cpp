#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h> // for error
#include <unistd.h>
#include <arpa/inet.h> //for Networking
#include <sys/socket.h> // for Networking
#include <netinet/ip.h> // for Networking
#include <string> // for string
#include <sstream> // for sstringstream parsing
#include "Database.h" //the thread safe database that was created
#include <thread> // to handle multiple clients
#include <functional> // to use reference to create reference.

void die(const char* msg){
    perror(msg);
    exit(EXIT_FAILURE);
}
void handle_client(int con_nfd,RCV::Database &DB){
    while(true){
        char buffer[1024]={0};
        
        ssize_t bytes=read(con_nfd,buffer,sizeof(buffer)-1);
        
        if(bytes<=0) {
            break;
        }

        std::string input(buffer);
        std::istringstream iss(input);
        std::string cmd,key,value ;
        
        iss>>cmd;

        if(cmd=="SET"){

            iss>>key;
            iss>>value;

            DB.set(key,value);

        }
        else if(cmd=="GET"){

            iss>>key;
            std::string out_value;

            if(DB.get(key,out_value)){
                out_value +="\n\r";
                write(con_nfd,out_value.data(),strlen(out_value.data()));

            }
            else
            {
               const char * response= "Not_Found_Any_Key\n";

                write(con_nfd,response,strlen(response));

            }
        }
        else{

            const char * response ="NOT correct Key \n";

            write(con_nfd,response,strlen(response));
            break;
        }
    }// while
    close(con_nfd);
    printf("Connection Closed on %d\n",con_nfd);

}
int main(){
    int fd=socket(AF_INET,SOCK_STREAM,0); // IPv4 + TCP

    if(fd < 0){
        die("SOCKET IS NOT FORMED");
    }

    RCV::Database DB;

    int val=1;
    setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val)); // SO_REUSEADDR MAKES SURE THAT WE CAN USE THE PORT AFTER THE OPRATION OR THE MAIN CODE ENDS.


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
        // now We will use our database

        // std::jthread client_thread(handle_client,con_nfd,DB); // this gives compilation error as shared mutex is not copyable or mutable
        std::jthread client_thread(handle_client,con_nfd,std::ref(DB));
        client_thread.detach();
    }
    return 0;
}

//tokenizer
//
