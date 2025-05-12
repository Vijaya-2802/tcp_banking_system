1.	Brief Description of the Program

This program simulates a basic banking transaction system using a client-server architecture with three main components: a client, a PhonePe module, and a UPI server module.
The client connects to the PhonePe file to authenticate with an OTP associated with a bank account number. Upon successful authentication, the client can choose to either check their balance or send money. 
The PhonePe module communicates with the UPI server to retrieve balance information or perform transactions. This program utilizes TCP connections to ensure reliable data transfer between modules.

2.	Design of modules and algorithms using pseudocode
The program has three main modules: Client, PhonePe, and UPI Server. Each module communicates over TCP and performs specific functions in the banking system.

2.1	Client.cpp
The Client module is responsible for user interaction, OTP verification, and transaction handling.

2.2	Phonepe.cpp
The PhonePe module is responsible for managing communication between the Client and the UPI Server. 
It performs crucial tasks such as verifying account details, generating and validating OTPs, and processing transactions such as balance inquiries and money transfers. 
The module listens for client connections, authenticates the client using the OTP, and forwards transaction requests to the UPI Server for processing. 
The interaction is done over TCP connections to ensure reliable communication. Here is the pseudocode for the Client module:

2.3	upi.cpp
The UPI Server module serves as the backend for processing financial transactions initiated by the client and validated by PhonePe.
This module handles essential tasks such as verifying bank account IDs, checking balances, and processing money transfers between accounts. 
It listens for incoming client requests, authenticates the account IDs, and interacts with the balance files to perform necessary transactions.

3.Learning outcomes
The UPI server project provides hands-on experience in network programming, file handling, and client-server communication using TCP sockets.
By implementing socket communication, we can understand how clients and servers interact over a network, as well as how to establish and manage connections.
The project also involves file handling, where account balances are stored and retrieved from text files, demonstrating how data persistence works in networked applications.
Additionally, the project covers input validation to ensure the integrity of user inputs, such as validating bank IDs before processing transactions. It also involves implementing basic security checks, such as verifying account details and ensuring sufficient funds for transactions. Error handling is incorporated, where the server sends appropriate responses to clients in case of invalid inputs or failed operations. Overall, the project provides a foundational understanding of building client-server applications with a focus on secure transactions and efficient communication.

4.References
[1] https://www.geeksforgeeks.org/socket-programming-in-cpp/
[2] https://medium.com/@naseefcse/ip-tcp-programming-for-beginners-using-c-5bafb3788001
