# 💬 Real-Time Chat Application – C (Socket Programming)

A terminal-based client-server chat application built using C and socket programming. Enables real-time two-way communication over a network using TCP/IP protocols.

---

## 📌 Features

- 🔗 Client-server architecture using TCP sockets
- 🗨️ Real-time text message exchange between client and server
- 💻 Terminal-based user interface
- 🔄 Full-duplex communication (both users can send/receive)
- ❌ Graceful exit and disconnect handling

---

## ⚙️ Technologies Used

- **Language:** C  
- **Networking:** POSIX Sockets (TCP)  
- **Libraries:** `<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<unistd.h>`, `<arpa/inet.h>`, `<netinet/in.h>`

---

## 🗂️ Project Structure

```bash
Real-time-chat-application/
├── client.c           # Client-side code
├── server.c           # Server-side code
└── README.md          # Project documentation 
```
---
🚀 How to Run
📦 Compile Both Programs
```bash
gcc server.c -o server
gcc client.c -o client
```
▶️ Start the Server
```bash
./server
```
Server will listen for incoming connections on a specified port.

▶️ Start the Client (in another terminal)
```bash
./client
```
Enter server IP (usually 127.0.0.1 for localhost) and port when prompted.

---
### 🖥️ Sample Output
## 📡 Server Terminal
```text
Server started. Waiting for connections...
Client connected!
Client: Hello, Server!
You: Hi there!
Client: Bye
You: Goodbye!
Connection closed.
```

## 💬 Client Terminal
```text
Enter server IP: 127.0.0.1
Enter port: 8080
Connected to the server.

You: Hello, Server!
Server: Hi there!
You: Bye
Server: Goodbye!
Disconnected from server.
```

---

## 🙋‍♂️ Author
Mohit Jain

Feel free to connect or collaborate!

- 🔗 [LinkedIn](https://www.linkedin.com/in/mohit-jain-dev/)  
- 💻 [GitHub](https://github.com/Mohitjain9654)  
- 🌐 [Portfolio Website](https://mohitjain-portfolio.vercel.app/)  
- 📧 Email: mohitjain965405@gmail.com

---
