#include<bits/stdc++.h>
#include<time.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include <sys/types.h> 
#include <stddef.h>
#include <sys/stat.h>
using namespace std;
#define port1 5001
#define port2 5002
int inet_serv_listen () {
    struct sockaddr_in servaddr;
    int sfd, len;
    int opt=1;
    if((sfd = socket(AF_INET, SOCK_STREAM, 0))==0) {
        perror("socket");
        exit(1);
    }
    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR  SO_REUSEPORT,&opt, sizeof(int))) { 
        perror("setsockopt"); 
        exit(1); 
    } 
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(port1);
    if(bind(sfd, (struct sockaddr*)&servaddr,sizeof(servaddr))<0) {
        perror("bind");
        exit(1);
    } 
    if(listen(sfd,10)<0){
        perror("listen");
        exit(1);
    }
    return sfd;
}
int tcp_client_conn () {
    int sfd=socket (AF_INET, SOCK_STREAM,0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(port2);
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.2"); 
    connect (sfd, (struct sockaddr*) & serv_addr,sizeof(serv_addr));
    return sfd;
} 
int generateOTP () {
    srand(static_cast<unsigned int> (time(0)));
    int otp=1000+(rand () %9000);
    return otp;
}

int main () {
    int sfd = inet_serv_listen();
    int psfd = tcp_client_conn();
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int nsfd = accept (sfd, (struct sockaddr*)&client_address, &client_address_len);
    if (nsfd == -1) {
        cerr << "Error accepting connection." << endl;
        close(sfd);
        return -1;
    }
    char id[9];
    recv(nsfd,id,sizeof(id),0);
    cout<<id<<endl;
    char flag[2];
    send(psfd,id,sizeof(id),0);
    recv(psfd,flag,sizeof(flag),0);
    flag[1]='\0';
    char valid[2] = "1", invalid[2] = "0";
    send(nsfd,flag,sizeof(flag),0);
    cout<<"----\n";
    cout<<flag<<endl;
    cout<<"----\n";
    if(strcmp(valid,flag)==0) 
{
        cout<<"Bank account exists"<<endl;
        string id_str(id);
        int rnum = generateOTP(); 
        cout<<id_str<<" "<<rnum<<endl;   
        string path="OTP/"+id_str+".txt";
        string otp=to_string(rnum);
        int fd = open(path.c_str(), O_WRONLY | O_CREAT, 0644); // Added O_CREAT flag and permissions        
if (fd < 0) {
            perror("Error opening file");
            close(nsfd);
            return -1;
        }
        write(fd,otp.c_str(),otp.length());
        close(fd);
        char cotp[5];
        recv(nsfd,cotp,sizeof(cotp),0);
        cotp[4] = '\0';
        if(strcmp(cotp,otp.c_str())==0)
        {
            cout<<"Authentication success"<<endl;
            send(nsfd,valid,sizeof(valid),0);
            
            int action;
            recv(nsfd, &action, sizeof(action), 0);
            send (psfd, &action, sizeof(action),0);
            action = ntohl(action);
            if(action==1) {
                int balance;
                recv(psfd, &balance, sizeof(balance), 0);
                send (nsfd, &balance, sizeof(balance), 0);
            }
            else
            {
                char receiver [9];
                int rlen = recv(nsfd,receiver,sizeof(receiver),0);
                receiver[rlen]='\0';
                cout<<receiver<<endl;
                char amount [25];
                int len = recv(nsfd, amount,25,0);
                cout<<len<<endl;
                amount[len]='\0';
                cout<<amount<<endl;
                send(psfd,receiver,rlen,0);
                sleep(2);
                send(psfd,amount,len,0);
                char status[25];
                recv(psfd,status,sizeof(status),0);
                send(nsfd,status,sizeof(status),0);
                
            }
        }
        else {
            cout<<"Authentication failed"<<endl;
            send(nsfd,invalid,sizeof(invalid),0);
        }
        
    }
    else {
        cout<<"Bank account doesn't exist "<<endl;  
    }
    return 0;
}
