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
#define port1 5002
vector<string> valid_ids = {"AB123456", "XY987654", "CD567890", "EF345678", "GH112233"};
int get_balance(const string& id_str) {
    string path = "BALANCE/" + id_str + ".txt";
    ifstream file(path);
    int balance = 0;
    if (!file.is_open()) {
        // File doesn't exist, create it with balance = 0
        ofstream new_file(path);
        if (new_file.is_open()) {
            new_file << "0"; // Initialize balance to zero
            new_file.close();
        }
    } else {
        // If file exists, read balance
        file >> balance;
        file.close();
    }
    return balance;
}

// Function to write balance to a file
void update_balance(const string& id_str, int balance) {
    string path = "BALANCE/" + id_str + ".txt";
    ofstream file(path, ios::trunc); // Overwrite the file
    if (file.is_open()) {
        file << balance;
        file.close();
    }
}

// Function to check if the ID format is valid
bool is_valid_bank_id_format(const string& id) {
    if (id.length() != 8) return false; // Bank ID should be exactly 8 characters
    
    // First two characters should be uppercase letters
    if (!isupper(id[0]) || !isupper(id[1])) return false;
    
    // Remaining six characters should be digits
    for (int i = 2; i < 8; ++i) {
        if (!isdigit(id[i])) return false;
    }
    return true;
}
int inet_serv_listen () {
    struct sockaddr_in servaddr;
    int sfd, len;
    int opt=1;
    if ((sfd = socket (AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket");
        exit (1);
    }
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(int))) 
    { 
        perror("setsockopt"); 
        exit (1); 
    } 
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.2"); 
    servaddr.sin_port = htons(port1);
    if(bind(sfd, (struct sockaddr*)&servaddr,sizeof(servaddr))<0)
   {
        perror("bind");
        exit(1);
    } 

    if(listen(sfd,10)<0)
    {
        perror("listen");
        exit(1);
    }
    return sfd;
}
int main()
{
	int sfd = inet_serv_listen ();
	struct sockaddr_in client_address;
	socklen_t client_address_len = sizeof(client_address);
	int npsfd = accept (sfd, (struct sockaddr*) & client_address, &client_address_len);
    if (npsfd == -1) {
		cerr << "Error accepting connection." << endl;
		close(sfd);
		return -1;
    }
    char id [9];
    recv(npsfd,id,sizeof(id),0);
    cout<<id<<endl;
    string received_id(id);
    received_id.erase(remove(received_id.begin(), received_id.end(), '\n'), received_id.end());
    cout<<received_id<<endl;
    string response = "0";
    if (is_valid_bank_id_format(received_id)) {
    	cout<<"PASS1\n";
        if(find(valid_ids.begin(), valid_ids.end(), received_id) != valid_ids.end()) {
        cout<<"PASS2\n";
            response = "1";
            send (npsfd, response.c_str(), response.size(), 0);
            cout<<"VALID USER\n";
            int action;
	      recv(npsfd, &action, sizeof(action), 0);
	     action = ntohl(action);
	     cout<<action<<endl;
if (action == 1) {
                int balance = get_balance(received_id);
                balance = htonl(balance);
                send(npsfd, &balance, sizeof(balance), 0);
            } else if (action == 2) {
            	cout<<"getting receiver info and amount\n";
                char receiver_id[9];
                recv(npsfd, receiver_id, sizeof(receiver_id), 0);
                char amount [25];
                int len = recv(npsfd, amount, sizeof(amount), 0);
                
                receiver_id [8] = '\0';
                string receiver_id_str(receiver_id);
                receiver_id_str.erase(remove(receiver_id_str.begin(), receiver_id_str.end(), '\n'), receiver_id_str.end());
                amount[len]='\0';
                cout<<receiver_id<<endl;
                cout<<amount<<endl;
                string amount1(amount);
                if (is_valid_bank_id_format(receiver_id_str) && 
                    find(valid_ids.begin(), valid_ids.end(), receiver_id_str) != valid_ids.end()) {
                    int sender_balance = get_balance(received_id);
                    int receiver_balance = get_balance(receiver_id_str);
                    if (sender_balance >= stoi(amount)) {
                        sender_balance -= stoi(amount1);
                        receiver_balance += stoi(amount1);
                        update_balance(received_id, sender_balance);
                        update_balance(receiver_id_str, receiver_balance);
                        string transfer_status = "Transfer successful";
                        send (npsfd, transfer_status.c_str(), transfer_status.size(), 0);
                    } else {
                        string transfer_status = "Insufficient balance";
                        send (npsfd, transfer_status.c_str(), transfer_status.size(), 0);
                    }
                } else {
                    // Send failure message (invalid receiver ID)
                    string transfer_status = "Invalid Receiver"; // Failure
                    send (npsfd, transfer_status.c_str(), transfer_status.size(), 0);
                }
                }
        }
        else {
        	send (npsfd, response.c_str(), response.size(), 0);	
        }
    }
    else {
    	send (npsfd, response.c_str(), response.size(), 0);
    }
    close(npsfd);
    close(sfd);
	return 0;
}
