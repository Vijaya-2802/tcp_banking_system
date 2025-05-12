#include<bits/stdc++.h>
#include<time.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/select.h>
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
int tcp_client_conn() {
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(port1);
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    connect(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    return sfd; }

int main () {   
    int sfd=tcp_client_conn(); 
    char bank_id[9],otp[5];
    cout<<"enter bank id\n";
    cin.getline(bank_id,9);
    cout<<bank_id<<endl;
    send(sfd,bank_id,sizeof(bank_id),0);
    char account_flag[2],af[2]="1";
    recv(sfd,account_flag,sizeof(account_flag),0);
    if(strcmp(account_flag,af)==0)  {
        cout<<"Account exists\n";
        cout<<"enter OTP"<<endl;
        cin.getline(otp,5);
        send(sfd,otp,sizeof(otp),0);
        char auth_flag[2];
        recv(sfd,auth_flag,sizeof(auth_flag),0);
        auth_flag[1]='\0';
        if(strcmp(auth_flag,af)==0) {
            cout<<"Authentication success"<<endl;
            cout<<"Enter 1 for checking balance or 2 for money transfer\n";
            int action;
            cin >> action;
            action = htonl(action);
            send(sfd, &action, sizeof(action), 0);
            if(ntohl(action) == 1) {
                int balance;
                recv(sfd, &balance, sizeof(balance), 0);
                balance = ntohl(balance); // Convert to host byte order
                cout << "Your balance is: " << balance << endl;
            } 
            else if(ntohl(action) == 2) {
                cout<<"Enter bank id you want to send money to:\n";
                string receiver_id;
                cin>>receiver_id;
                cout<<receiver_id<<endl;
                string amount;
                cout << "Enter amount to transfer: \n";
                cin >> amount;
                cout<<amount<<endl;
                cout<<amount.length()<<endl;
                cout<<amount.c_str()<<endl;
                send(sfd, receiver_id.c_str(),sizeof(receiver_id.c_str()),0);
                sleep(2);
                send(sfd, amount.c_str(), amount.length(), 0);
                char transfer_status[25];
                recv(sfd, transfer_status, sizeof(transfer_status), 0);
                cout<<"transfer status: "<<transfer_status<<endl;
            }
        }
        else {
            cout<<"Authentication failure"<<endl;
        }
    }
    else {
        cout<<"Account doesn't exists\n";
    }
    return 0;
}
